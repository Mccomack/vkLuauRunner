module;

export module property;

import std;

export template <typename T>
class Property {
   private:
    T value = T{};
    std::function<T(const T& now)> getter = nullptr;
    std::function<T(const T& old, const T& req)> setter = nullptr;

   public:
    Property(
        T init = T{},
        std::function<T(const T&)> getter = nullptr,
        std::function<T(const T& old, const T& req)> setter = nullptr
    ) noexcept
        : value(init),
          getter(getter),
          setter(setter) {};

    Property& operator=(const T& requestedValue) {
        if (setter) {
            value = setter(value, requestedValue);

            return *this;
        }

        value = requestedValue;

        return *this;
    }

    operator T() const { return getter ? getter(value) : value; }

    void bindGet(std::function<T(const T& now)> func) noexcept {
        getter = func;
    }
    void bindSet(std::function<T(const T& old, const T& req)> func) noexcept {
        setter = func;
    }

    const T rawGet() noexcept { return value; }
    void rawSet(const T& _val) noexcept { value = _val; }
};
