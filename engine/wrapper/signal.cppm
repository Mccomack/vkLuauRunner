module;

export module signal;

import std;

export template <typename... Args>
class Signal {
   public:
    using Callback = std::function<void(Args...)>;
    using Id = std::uint64_t;

    class Connection {
       private:
        Signal* signal;
        Id id;

       public:
        Connection() noexcept : signal(nullptr), id(0) {}
        Connection(Signal* signal_, Id id_) noexcept
            : signal(signal_), id(id_) {}

        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;

        Connection(Connection&& other) noexcept
            : signal(other.signal), id(other.id) {
            other.signal = nullptr;
        }

        Connection& operator=(Connection&& other) noexcept {
            if (this == &other)
                return *this;

            disconnect();

            signal = other.signal;
            id = other.id;
            other.signal = nullptr;

            return *this;
        }

        ~Connection() { disconnect(); }

        void disconnect() {
            if (!signal)
                return;

            signal->disconnect(id);
            signal = nullptr;
        }
    };

   private:
    Id nextId = 1;
    std::unordered_map<Id, Callback> callbacks;

   public:
    Signal() = default;

    Signal(const Signal&) = delete;
    Signal& operator=(const Signal&) = delete;

    Signal(Signal&&) = delete;
    Signal& operator=(Signal&&) = delete;

    [[nodiscard]] Connection connect(Callback callback) {
        Id id = nextId++;
        callbacks.emplace(id, std::move(callback));

        return Connection(this, id);
    }

    void disconnect(Id id) { callbacks.erase(id); }

    void fire(Args... args) const {
        std::vector<Callback> snapshot;
        snapshot.reserve(callbacks.size());

        for (const auto& [id, callback] : callbacks)
            snapshot.push_back(callback);

        for (const auto& callback : snapshot)
            callback(args...);
    }
};
