module;
//

#define INPUT_STATUS \
    X(eNone)         \
    X(eRelease)      \
    X(ePress)        \
    X(eRepeat)

export module input:common;

export namespace input {
#define X(name) name,
    enum Status { INPUT_STATUS };
#undef X

    //
}