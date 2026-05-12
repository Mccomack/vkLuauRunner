module;
#include <lua.h>
#include <lualib.h>

export module luau:object.base;
import :object.event;
import :taskScheduler;
import :common;

import property;

import std;

namespace luau::object {
    export class Base {
       private:
        //

       protected:
        std::unordered_set<std::string> classNames;

        static void raiseChangedEvent(const Base* self);

       public:
        Base();
        Property<int> coolvalue = 0;
        Property<std::string> className;
        void bindOnChanged(std::function<void(void)> func);
    };
}

template <typename T>
void warpChanged(
    const luau::object::Base* self,
    Property<T>* prop,
    std::function<void(const luau::object::Base*)> raiseChangedEvent,
    std::string propertyName
) {
    prop->bindSet(
        [self, prop, raiseChangedEvent](const T& old, const T& req) -> T {
            prop->rawSet(req);

            raiseChangedEvent(self);

            return req;
        }
    );
}

luau::object::Base::Base() {
    className = "Base";

    // clang-format off
    warpChanged<int>(this, &coolvalue, raiseChangedEvent, "coolvalue");
    // clang-format on
}

void luau::object::Base::raiseChangedEvent(const Base* self) {
    std::cout << "raiseChangedEvent: " << self->coolvalue << std::endl;
}

void luau::object::Base::bindOnChanged(std::function<void(void)> func) {
    //
}