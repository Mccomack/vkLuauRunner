module;

export module luau:taskScheduler;

import std;

export namespace luau {
    class TaskScheduler {
       private:
        TaskScheduler() = default;
        ~TaskScheduler() = default;

        TaskScheduler(const TaskScheduler&) = delete;
        TaskScheduler& operator=(const TaskScheduler&) = delete;

        //
       public:
        static TaskScheduler& getInstance();
    };

    //
}

luau::TaskScheduler& luau::TaskScheduler::getInstance() {
    static TaskScheduler instance;

    return instance;
}

// TODO: fill this file