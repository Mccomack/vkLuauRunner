export module signal_safe;

import std;

export template <typename... Args>
class SafeSignal {
   public:
    using Callback = std::function<void(Args...)>;
    using Id = std::uint64_t;

   private:
    struct Slot {
        Id id;
        Callback fn;
        std::atomic<bool> alive{true};
        std::recursive_mutex callMutex;  // 콜백 실행 중에 잡고 있음

        Slot(Id id_, Callback fn_)
            : id(id_),
              fn(std::move(fn_)) {}
    };

    using SlotList = std::vector<std::shared_ptr<Slot>>;

    struct State {
        mutable std::mutex mutex;
        std::shared_ptr<const SlotList> slots =
            std::make_shared<const SlotList>();
        Id nextId = 1;

        Id add(Callback cb) {
            std::shared_ptr<const SlotList> old;
            Id id;
            {
                std::lock_guard lock(mutex);
                id = nextId++;
                auto next = std::make_shared<SlotList>(*slots);
                next->push_back(std::make_shared<Slot>(id, std::move(cb)));
                old = std::exchange(slots, std::move(next));
            }
            return id;
        }

        // 제거된 슬롯을 반환해서 호출자가 실행 중인 콜백을 기다릴 수 있게 함
        std::shared_ptr<Slot> remove(Id id) {
            std::shared_ptr<const SlotList> old;
            std::shared_ptr<Slot> removed;
            {
                std::lock_guard lock(mutex);

                auto it = std::ranges::find(*slots, id, [](const auto& s) {
                    return s->id;
                });
                if (it == slots->end())
                    return nullptr;

                removed = *it;
                removed->alive.store(false, std::memory_order_release);

                auto next = std::make_shared<SlotList>();
                next->reserve(slots->size() - 1);
                for (const auto& s : *slots)
                    if (s->id != id)
                        next->push_back(s);

                old = std::exchange(slots, std::move(next));
            }
            return removed;
        }

        std::shared_ptr<const SlotList> snapshot() const {
            std::lock_guard lock(mutex);
            return slots;
        }
    };

    std::shared_ptr<State> state = std::make_shared<State>();

   public:
    class Connection {
       private:
        std::weak_ptr<State> state;
        Id id = 0;

       public:
        Connection() noexcept = default;
        Connection(std::weak_ptr<State> state_, Id id_) noexcept
            : state(std::move(state_)),
              id(id_) {}

        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
        Connection(Connection&&) noexcept = default;

        Connection& operator=(Connection&& other) noexcept {
            if (this != &other) {
                disconnect();
                state = std::move(other.state);
                id = other.id;
            }
            return *this;
        }

        ~Connection() { disconnect(); }

        void disconnect() {
            if (auto s = state.lock()) {
                if (auto slot = s->remove(id)) {
                    // 다른 스레드에서 실행 중인 콜백이 끝날 때까지 대기.
                    // 같은 스레드(콜백 안에서 자기 자신/재진입)라면 recursive라
                    // 통과.
                    std::lock_guard wait(slot->callMutex);
                }
            }
            state.reset();
        }
    };

    SafeSignal() = default;
    SafeSignal(const SafeSignal&) = delete;
    SafeSignal& operator=(const SafeSignal&) = delete;
    SafeSignal(SafeSignal&&) = delete;
    SafeSignal& operator=(SafeSignal&&) = delete;

    [[nodiscard]] Connection connect(Callback callback) {
        return Connection(state, state->add(std::move(callback)));
    }

    void fire(Args... args) const {
        auto list = state->snapshot();
        for (const auto& slot : *list) {
            std::lock_guard call(slot->callMutex);
            if (slot->alive.load(
                    std::memory_order_acquire
                ))  // 락 잡은 뒤에 확인
                slot->fn(args...);
        }
    }
};
