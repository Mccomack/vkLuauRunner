export module defer;

import std;

class defer {
   private:
    std::function<void()> func;
    bool cancel;

   public:
    defer(std::function<void()> f)
        : func(f) {}
    ~defer() {
        if (cancel)
            func();
    }

    void Cancel() { cancel = true; }
    bool isCancled() { return cancel; }
};
