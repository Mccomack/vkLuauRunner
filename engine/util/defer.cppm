export module defer;

import std;

export class defer {
   private:
    std::function<void()> func;
    bool cancelled = false;

   public:
    defer(std::function<void()> f)
        : func(f) {}
    ~defer() {
        if (!cancelled)
            func();
    }

    void Cancel() { cancelled = true; }
    bool isCancled() { return cancelled; }
};
