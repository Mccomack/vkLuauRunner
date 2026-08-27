module;
#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>

#define KEYBOARD_KEYS \
    X(Unknown)        \
    X(Space)          \
    X(Apostrophe)     \
    X(Comma)          \
    X(Minus)          \
    X(Period)         \
    X(Slash)          \
    X(Num0)           \
    X(Num1)           \
    X(Num2)           \
    X(Num3)           \
    X(Num4)           \
    X(Num5)           \
    X(Num6)           \
    X(Num7)           \
    X(Num8)           \
    X(Num9)           \
    X(Semicolon)      \
    X(Equal)          \
    X(A)              \
    X(B)              \
    X(C)              \
    X(D)              \
    X(E)              \
    X(F)              \
    X(G)              \
    X(H)              \
    X(I)              \
    X(J)              \
    X(K)              \
    X(L)              \
    X(M)              \
    X(N)              \
    X(O)              \
    X(P)              \
    X(Q)              \
    X(R)              \
    X(S)              \
    X(T)              \
    X(U)              \
    X(V)              \
    X(W)              \
    X(X)              \
    X(Y)              \
    X(Z)              \
    X(LeftBracket)    \
    X(Backslash)      \
    X(RightBracket)   \
    X(GraveAccent)    \
    X(Escape)         \
    X(Enter)          \
    X(Tab)            \
    X(Backspace)      \
    X(Insert)         \
    X(Delete)         \
    X(ArrowRight)     \
    X(ArrowLeft)      \
    X(ArrowDown)      \
    X(ArrowUp)        \
    X(PageUp)         \
    X(PageDown)       \
    X(Home)           \
    X(End)            \
    X(CapsLock)       \
    X(ScrollLock)     \
    X(NumLock)        \
    X(PrintScreen)    \
    X(Pause)          \
    X(F1)             \
    X(F2)             \
    X(F3)             \
    X(F4)             \
    X(F5)             \
    X(F6)             \
    X(F7)             \
    X(F8)             \
    X(F9)             \
    X(F10)            \
    X(F11)            \
    X(F12)            \
    X(NumPad0)        \
    X(NumPad1)        \
    X(NumPad2)        \
    X(NumPad3)        \
    X(NumPad4)        \
    X(NumPad5)        \
    X(NumPad6)        \
    X(NumPad7)        \
    X(NumPad8)        \
    X(NumPad9)        \
    X(NumPadDecimal)  \
    X(NumPadDivide)   \
    X(NumPadMultiply) \
    X(NumPadSubtract) \
    X(NumPadAdd)      \
    X(NumPadEnter)    \
    X(NumPadEqual)    \
    X(LeftShift)      \
    X(LeftControl)    \
    X(LeftAlt)        \
    X(LeftSuper)      \
    X(RightShift)     \
    X(RightControl)   \
    X(RightAlt)       \
    X(RightSuper)     \
    X(Menu)

export module input:keyboard;

import bimap;

import std;

namespace input {
#define X(key) key,
    export enum keyboard { KEYBOARD_KEYS };
#undef X

    bimap<input::keyboard, int> keyboardMap;

    bool isKeyPressed(input::keyboard key);
}

bool input::isKeyPressed(input::keyboard key) {
    const bool* keyStatus = SDL_GetKeyboardState(nullptr);

    return keyStatus[input::keyboardMap.getB(key)];
}

static int _ = []() -> int {
    // clang-format off
    input::keyboardMap.insert(input::keyboard::Unknown, SDL_SCANCODE_UNKNOWN);
    input::keyboardMap.insert(input::keyboard::Space, SDL_SCANCODE_SPACE);
    input::keyboardMap.insert(input::keyboard::Apostrophe, SDL_SCANCODE_APOSTROPHE);
    input::keyboardMap.insert(input::keyboard::Comma, SDL_SCANCODE_COMMA);
    input::keyboardMap.insert(input::keyboard::Minus, SDL_SCANCODE_MINUS);
    input::keyboardMap.insert(input::keyboard::Period, SDL_SCANCODE_PERIOD);
    input::keyboardMap.insert(input::keyboard::Slash, SDL_SCANCODE_SLASH);
    input::keyboardMap.insert(input::keyboard::Num0, SDL_SCANCODE_0);
    input::keyboardMap.insert(input::keyboard::Num1, SDL_SCANCODE_1);
    input::keyboardMap.insert(input::keyboard::Num2, SDL_SCANCODE_2);
    input::keyboardMap.insert(input::keyboard::Num3, SDL_SCANCODE_3);
    input::keyboardMap.insert(input::keyboard::Num4, SDL_SCANCODE_4);
    input::keyboardMap.insert(input::keyboard::Num5, SDL_SCANCODE_5);
    input::keyboardMap.insert(input::keyboard::Num6, SDL_SCANCODE_6);
    input::keyboardMap.insert(input::keyboard::Num7, SDL_SCANCODE_7);
    input::keyboardMap.insert(input::keyboard::Num8, SDL_SCANCODE_8);
    input::keyboardMap.insert(input::keyboard::Num9, SDL_SCANCODE_9);
    input::keyboardMap.insert(input::keyboard::Semicolon, SDL_SCANCODE_SEMICOLON);
    input::keyboardMap.insert(input::keyboard::Equal, SDL_SCANCODE_EQUALS);
    input::keyboardMap.insert(input::keyboard::A, SDL_SCANCODE_A);
    input::keyboardMap.insert(input::keyboard::B, SDL_SCANCODE_B);
    input::keyboardMap.insert(input::keyboard::C, SDL_SCANCODE_C);
    input::keyboardMap.insert(input::keyboard::D, SDL_SCANCODE_D);
    input::keyboardMap.insert(input::keyboard::E, SDL_SCANCODE_E);
    input::keyboardMap.insert(input::keyboard::F, SDL_SCANCODE_F);
    input::keyboardMap.insert(input::keyboard::G, SDL_SCANCODE_G);
    input::keyboardMap.insert(input::keyboard::H, SDL_SCANCODE_H);
    input::keyboardMap.insert(input::keyboard::I, SDL_SCANCODE_I);
    input::keyboardMap.insert(input::keyboard::J, SDL_SCANCODE_J);
    input::keyboardMap.insert(input::keyboard::K, SDL_SCANCODE_K);
    input::keyboardMap.insert(input::keyboard::L, SDL_SCANCODE_L);
    input::keyboardMap.insert(input::keyboard::M, SDL_SCANCODE_M);
    input::keyboardMap.insert(input::keyboard::N, SDL_SCANCODE_N);
    input::keyboardMap.insert(input::keyboard::O, SDL_SCANCODE_O);
    input::keyboardMap.insert(input::keyboard::P, SDL_SCANCODE_P);
    input::keyboardMap.insert(input::keyboard::Q, SDL_SCANCODE_Q);
    input::keyboardMap.insert(input::keyboard::R, SDL_SCANCODE_R);
    input::keyboardMap.insert(input::keyboard::S, SDL_SCANCODE_S);
    input::keyboardMap.insert(input::keyboard::T, SDL_SCANCODE_T);
    input::keyboardMap.insert(input::keyboard::U, SDL_SCANCODE_U);
    input::keyboardMap.insert(input::keyboard::V, SDL_SCANCODE_V);
    input::keyboardMap.insert(input::keyboard::W, SDL_SCANCODE_W);
    input::keyboardMap.insert(input::keyboard::X, SDL_SCANCODE_X);
    input::keyboardMap.insert(input::keyboard::Y, SDL_SCANCODE_Y);
    input::keyboardMap.insert(input::keyboard::Z, SDL_SCANCODE_Z);
    input::keyboardMap.insert(input::keyboard::LeftBracket, SDL_SCANCODE_LEFTBRACKET);
    input::keyboardMap.insert(input::keyboard::Backslash, SDL_SCANCODE_BACKSLASH);
    input::keyboardMap.insert(input::keyboard::RightBracket, SDL_SCANCODE_RIGHTBRACKET);
    input::keyboardMap.insert(input::keyboard::GraveAccent, SDL_SCANCODE_GRAVE);
    input::keyboardMap.insert(input::keyboard::Escape, SDL_SCANCODE_ESCAPE);
    input::keyboardMap.insert(input::keyboard::Enter, SDL_SCANCODE_RETURN);
    input::keyboardMap.insert(input::keyboard::Tab, SDL_SCANCODE_TAB);
    input::keyboardMap.insert(input::keyboard::Backspace, SDL_SCANCODE_BACKSPACE);
    input::keyboardMap.insert(input::keyboard::Insert, SDL_SCANCODE_INSERT);
    input::keyboardMap.insert(input::keyboard::Delete, SDL_SCANCODE_DELETE);
    input::keyboardMap.insert(input::keyboard::ArrowRight, SDL_SCANCODE_RIGHT);
    input::keyboardMap.insert(input::keyboard::ArrowLeft, SDL_SCANCODE_LEFT);
    input::keyboardMap.insert(input::keyboard::ArrowDown, SDL_SCANCODE_DOWN);
    input::keyboardMap.insert(input::keyboard::ArrowUp, SDL_SCANCODE_UP);
    input::keyboardMap.insert(input::keyboard::PageUp, SDL_SCANCODE_PAGEUP);
    input::keyboardMap.insert(input::keyboard::PageDown, SDL_SCANCODE_PAGEDOWN);
    input::keyboardMap.insert(input::keyboard::Home, SDL_SCANCODE_HOME);
    input::keyboardMap.insert(input::keyboard::End, SDL_SCANCODE_END);
    input::keyboardMap.insert(input::keyboard::CapsLock, SDL_SCANCODE_CAPSLOCK);
    input::keyboardMap.insert(input::keyboard::ScrollLock, SDL_SCANCODE_SCROLLLOCK);
    input::keyboardMap.insert(input::keyboard::NumLock, SDL_SCANCODE_NUMLOCKCLEAR);
    input::keyboardMap.insert(input::keyboard::PrintScreen, SDL_SCANCODE_PRINTSCREEN);
    input::keyboardMap.insert(input::keyboard::Pause, SDL_SCANCODE_PAUSE);
    input::keyboardMap.insert(input::keyboard::F1, SDL_SCANCODE_F1);
    input::keyboardMap.insert(input::keyboard::F2, SDL_SCANCODE_F2);
    input::keyboardMap.insert(input::keyboard::F3, SDL_SCANCODE_F3);
    input::keyboardMap.insert(input::keyboard::F4, SDL_SCANCODE_F4);
    input::keyboardMap.insert(input::keyboard::F5, SDL_SCANCODE_F5);
    input::keyboardMap.insert(input::keyboard::F6, SDL_SCANCODE_F6);
    input::keyboardMap.insert(input::keyboard::F7, SDL_SCANCODE_F7);
    input::keyboardMap.insert(input::keyboard::F8, SDL_SCANCODE_F8);
    input::keyboardMap.insert(input::keyboard::F9, SDL_SCANCODE_F9);
    input::keyboardMap.insert(input::keyboard::F10, SDL_SCANCODE_F10);
    input::keyboardMap.insert(input::keyboard::F11, SDL_SCANCODE_F11);
    input::keyboardMap.insert(input::keyboard::F12, SDL_SCANCODE_F12);
    input::keyboardMap.insert(input::keyboard::NumPad0, SDL_SCANCODE_KP_0);
    input::keyboardMap.insert(input::keyboard::NumPad1, SDL_SCANCODE_KP_1);
    input::keyboardMap.insert(input::keyboard::NumPad2, SDL_SCANCODE_KP_2);
    input::keyboardMap.insert(input::keyboard::NumPad3, SDL_SCANCODE_KP_3);
    input::keyboardMap.insert(input::keyboard::NumPad4, SDL_SCANCODE_KP_4);
    input::keyboardMap.insert(input::keyboard::NumPad5, SDL_SCANCODE_KP_5);
    input::keyboardMap.insert(input::keyboard::NumPad6, SDL_SCANCODE_KP_6);
    input::keyboardMap.insert(input::keyboard::NumPad7, SDL_SCANCODE_KP_7);
    input::keyboardMap.insert(input::keyboard::NumPad8, SDL_SCANCODE_KP_8);
    input::keyboardMap.insert(input::keyboard::NumPad9, SDL_SCANCODE_KP_9);
    input::keyboardMap.insert(input::keyboard::NumPadDecimal, SDL_SCANCODE_KP_DECIMAL);
    input::keyboardMap.insert(input::keyboard::NumPadDivide, SDL_SCANCODE_KP_DIVIDE);
    input::keyboardMap.insert(input::keyboard::NumPadMultiply, SDL_SCANCODE_KP_MULTIPLY);
    input::keyboardMap.insert(input::keyboard::NumPadSubtract, SDL_SCANCODE_KP_MINUS);
    input::keyboardMap.insert(input::keyboard::NumPadAdd, SDL_SCANCODE_KP_PLUS);
    input::keyboardMap.insert(input::keyboard::NumPadEnter, SDL_SCANCODE_KP_ENTER);
    input::keyboardMap.insert(input::keyboard::NumPadEqual, SDL_SCANCODE_KP_EQUALS);
    input::keyboardMap.insert(input::keyboard::LeftShift, SDL_SCANCODE_LSHIFT);
    input::keyboardMap.insert(input::keyboard::LeftControl, SDL_SCANCODE_LCTRL);
    input::keyboardMap.insert(input::keyboard::LeftAlt, SDL_SCANCODE_LALT);
    input::keyboardMap.insert(input::keyboard::LeftSuper, SDL_SCANCODE_LGUI);
    input::keyboardMap.insert(input::keyboard::RightShift, SDL_SCANCODE_RSHIFT);
    input::keyboardMap.insert(input::keyboard::RightControl, SDL_SCANCODE_RCTRL);
    input::keyboardMap.insert(input::keyboard::RightAlt, SDL_SCANCODE_RALT);
    input::keyboardMap.insert(input::keyboard::RightSuper, SDL_SCANCODE_RGUI);
    input::keyboardMap.insert(input::keyboard::Menu, SDL_SCANCODE_MENU);
    // clang-format on

    return 0;
}();