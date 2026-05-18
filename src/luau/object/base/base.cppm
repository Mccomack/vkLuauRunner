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
        void bindOnChangedEvent(std::function<void(void)> func);
    };
}

using Base = luau::object::Base;

template <typename T>
void warpChanged(
    const Base* self,
    Property<T>* prop,
    std::function<void(const Base*)> raiseChangedEvent,
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

Base::Base() {
    className = "Base";
    classNames.insert("Base");

    // clang-format off
    warpChanged<int>(this, &coolvalue, raiseChangedEvent, "coolvalue");
    // clang-format on
}

void Base::raiseChangedEvent(const Base* self) {
    std::cout << "raiseChangedEvent: " << self->coolvalue << std::endl;
}

void Base::bindOnChangedEvent(std::function<void(void)> func) {
    //
}