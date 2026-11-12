export module signal;

import std;

export template <typename... Args>
class Signal {
   public:
    using Callback = std::function<void(Args...)>;
    using Id = std::uint64_t;

   private:
    struct State {
        struct Slot {
            Id id;
            Callback fn;  // 비어 있으면 삭제 예약된 슬롯
        };

        // fire 중 push_back 해도 실행 중인 요소의 참조 유지
        std::deque<Slot> slots;
        Id nextId = 1;
        int firing = 0;
        bool dirty = false;

        void remove(Id id) {
            auto it = std::ranges::find(slots, id, &Slot::id);
            if (it == slots.end())
                return;

            if (firing > 0) {
                it->fn = nullptr;  // 지연 삭제
                dirty = true;
            } else {
                slots.erase(it);
            }
        }

        void compact() {
            if (firing == 0 && dirty) {
                std::erase_if(slots, [](const Slot& s) { return !s.fn; });
                dirty = false;
            }
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
            if (auto s = state.lock())
                s->remove(id);
            state.reset();
        }
    };

    Signal() = default;
    Signal(const Signal&) = delete;
    Signal& operator=(const Signal&) = delete;
    Signal(Signal&&) = delete;
    Signal& operator=(Signal&&) = delete;

    [[nodiscard]] Connection connect(Callback callback) {
        Id id = state->nextId++;
        state->slots.push_back({id, std::move(callback)});
        return Connection(state, id);
    }

    void fire(Args... args) const {
        auto s = state;  // 콜백 안에서 Signal이 파괴돼도 State는 유지
        const auto count =
            s->slots.size();  // fire 중 새로 연결된 건 이번엔 호출 안 함

        ++s->firing;
        struct Guard {
            State& s;
            ~Guard() {
                --s.firing;
                s.compact();
            }
        } guard{*s};

        for (std::size_t i = 0; i < count; ++i) {
            auto& slot = s->slots[i];
            if (slot.fn)  // 중간에 disconnect된 슬롯은 건너뜀
                slot.fn(args...);
        }
    }
};
