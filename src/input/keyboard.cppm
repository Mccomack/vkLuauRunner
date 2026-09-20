module;
#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>

#define KEYBOARD_KEYS                           \
    X(Unknown, SDL_SCANCODE_UNKNOWN)            \
    X(Space, SDL_SCANCODE_SPACE)                \
    X(Apostrophe, SDL_SCANCODE_APOSTROPHE)      \
    X(Comma, SDL_SCANCODE_COMMA)                \
    X(Minus, SDL_SCANCODE_MINUS)                \
    X(Period, SDL_SCANCODE_PERIOD)              \
    X(Slash, SDL_SCANCODE_SLASH)                \
    X(Num0, SDL_SCANCODE_0)                     \
    X(Num1, SDL_SCANCODE_1)                     \
    X(Num2, SDL_SCANCODE_2)                     \
    X(Num3, SDL_SCANCODE_3)                     \
    X(Num4, SDL_SCANCODE_4)                     \
    X(Num5, SDL_SCANCODE_5)                     \
    X(Num6, SDL_SCANCODE_6)                     \
    X(Num7, SDL_SCANCODE_7)                     \
    X(Num8, SDL_SCANCODE_8)                     \
    X(Num9, SDL_SCANCODE_9)                     \
    X(Semicolon, SDL_SCANCODE_SEMICOLON)        \
    X(Equal, SDL_SCANCODE_EQUALS)               \
    X(A, SDL_SCANCODE_A)                        \
    X(B, SDL_SCANCODE_B)                        \
    X(C, SDL_SCANCODE_C)                        \
    X(D, SDL_SCANCODE_D)                        \
    X(E, SDL_SCANCODE_E)                        \
    X(F, SDL_SCANCODE_F)                        \
    X(G, SDL_SCANCODE_G)                        \
    X(H, SDL_SCANCODE_H)                        \
    X(I, SDL_SCANCODE_I)                        \
    X(J, SDL_SCANCODE_J)                        \
    X(K, SDL_SCANCODE_K)                        \
    X(L, SDL_SCANCODE_L)                        \
    X(M, SDL_SCANCODE_M)                        \
    X(N, SDL_SCANCODE_N)                        \
    X(O, SDL_SCANCODE_O)                        \
    X(P, SDL_SCANCODE_P)                        \
    X(Q, SDL_SCANCODE_Q)                        \
    X(R, SDL_SCANCODE_R)                        \
    X(S, SDL_SCANCODE_S)                        \
    X(T, SDL_SCANCODE_T)                        \
    X(U, SDL_SCANCODE_U)                        \
    X(V, SDL_SCANCODE_V)                        \
    X(W, SDL_SCANCODE_W)                        \
    X(X, SDL_SCANCODE_X)                        \
    X(Y, SDL_SCANCODE_Y)                        \
    X(Z, SDL_SCANCODE_Z)                        \
    X(LeftBracket, SDL_SCANCODE_LEFTBRACKET)    \
    X(Backslash, SDL_SCANCODE_BACKSLASH)        \
    X(RightBracket, SDL_SCANCODE_RIGHTBRACKET)  \
    X(GraveAccent, SDL_SCANCODE_GRAVE)          \
    X(Escape, SDL_SCANCODE_ESCAPE)              \
    X(Enter, SDL_SCANCODE_RETURN)               \
    X(Tab, SDL_SCANCODE_TAB)                    \
    X(Backspace, SDL_SCANCODE_BACKSPACE)        \
    X(Insert, SDL_SCANCODE_INSERT)              \
    X(Delete, SDL_SCANCODE_DELETE)              \
    X(ArrowRight, SDL_SCANCODE_RIGHT)           \
    X(ArrowLeft, SDL_SCANCODE_LEFT)             \
    X(ArrowDown, SDL_SCANCODE_DOWN)             \
    X(ArrowUp, SDL_SCANCODE_UP)                 \
    X(PageUp, SDL_SCANCODE_PAGEUP)              \
    X(PageDown, SDL_SCANCODE_PAGEDOWN)          \
    X(Home, SDL_SCANCODE_HOME)                  \
    X(End, SDL_SCANCODE_END)                    \
    X(CapsLock, SDL_SCANCODE_CAPSLOCK)          \
    X(ScrollLock, SDL_SCANCODE_SCROLLLOCK)      \
    X(NumLock, SDL_SCANCODE_NUMLOCKCLEAR)       \
    X(PrintScreen, SDL_SCANCODE_PRINTSCREEN)    \
    X(Pause, SDL_SCANCODE_PAUSE)                \
    X(F1, SDL_SCANCODE_F1)                      \
    X(F2, SDL_SCANCODE_F2)                      \
    X(F3, SDL_SCANCODE_F3)                      \
    X(F4, SDL_SCANCODE_F4)                      \
    X(F5, SDL_SCANCODE_F5)                      \
    X(F6, SDL_SCANCODE_F6)                      \
    X(F7, SDL_SCANCODE_F7)                      \
    X(F8, SDL_SCANCODE_F8)                      \
    X(F9, SDL_SCANCODE_F9)                      \
    X(F10, SDL_SCANCODE_F10)                    \
    X(F11, SDL_SCANCODE_F11)                    \
    X(F12, SDL_SCANCODE_F12)                    \
    X(NumPad0, SDL_SCANCODE_KP_0)               \
    X(NumPad1, SDL_SCANCODE_KP_1)               \
    X(NumPad2, SDL_SCANCODE_KP_2)               \
    X(NumPad3, SDL_SCANCODE_KP_3)               \
    X(NumPad4, SDL_SCANCODE_KP_4)               \
    X(NumPad5, SDL_SCANCODE_KP_5)               \
    X(NumPad6, SDL_SCANCODE_KP_6)               \
    X(NumPad7, SDL_SCANCODE_KP_7)               \
    X(NumPad8, SDL_SCANCODE_KP_8)               \
    X(NumPad9, SDL_SCANCODE_KP_9)               \
    X(NumPadDecimal, SDL_SCANCODE_KP_DECIMAL)   \
    X(NumPadDivide, SDL_SCANCODE_KP_DIVIDE)     \
    X(NumPadMultiply, SDL_SCANCODE_KP_MULTIPLY) \
    X(NumPadSubtract, SDL_SCANCODE_KP_MINUS)    \
    X(NumPadAdd, SDL_SCANCODE_KP_PLUS)          \
    X(NumPadEnter, SDL_SCANCODE_KP_ENTER)       \
    X(NumPadEqual, SDL_SCANCODE_KP_EQUALS)      \
    X(LeftShift, SDL_SCANCODE_LSHIFT)           \
    X(LeftControl, SDL_SCANCODE_LCTRL)          \
    X(LeftAlt, SDL_SCANCODE_LALT)               \
    X(LeftSuper, SDL_SCANCODE_LGUI)             \
    X(RightShift, SDL_SCANCODE_RSHIFT)          \
    X(RightControl, SDL_SCANCODE_RCTRL)         \
    X(RightAlt, SDL_SCANCODE_RALT)              \
    X(RightSuper, SDL_SCANCODE_RGUI)            \
    X(Menu, SDL_SCANCODE_MENU)

export module input:keyboard;

import bimap;

import std;

namespace input {
#define X(key, scancode) key,
    export enum keyboard { KEYBOARD_KEYS Count };
#undef X

    inline constexpr auto keyToScancode = []() {
        std::array<SDL_Scancode, static_cast<size_t>(keyboard::Count)> arr{};
#define X(key, scancode) arr[static_cast<size_t>(keyboard::key)] = scancode;
        KEYBOARD_KEYS
#undef X
        return arr;
    }();

    inline constexpr auto scancodeToKey = []() {
        std::array<keyboard, SDL_SCANCODE_COUNT> arr{};
        arr.fill(keyboard::Unknown);
#define X(key, scancode) arr[static_cast<size_t>(scancode)] = keyboard::key;
        KEYBOARD_KEYS
#undef X
        return arr;
    }();

    constexpr SDL_Scancode toScancode(keyboard key) {
        return keyToScancode[static_cast<size_t>(key)];
    }

    constexpr keyboard fromScancode(SDL_Scancode sc) {
        return scancodeToKey[static_cast<size_t>(sc)];
    }

    bool isKeyPressed(input::keyboard key);
}

bool input::isKeyPressed(input::keyboard key) {
    const bool* keyStatus = SDL_GetKeyboardState(nullptr);
    return keyStatus[toScancode(key)];
}
