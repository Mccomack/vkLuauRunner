module;
#include <GLFW/glfw3.h>

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

import std;

namespace input {
#define X(key) key,
    export enum keyboard { KEYBOARD_KEYS };
#undef X

    const std::unordered_map<int, keyboard> getGlfwKeyMap();
    const std::unordered_map<keyboard, int> getKeyboardKeyMap();
}

const std::unordered_map<int, input::keyboard> input::getGlfwKeyMap() {
    static const std::unordered_map<int, input::keyboard> glfwKeyMap = {
        {GLFW_KEY_UNKNOWN, input::keyboard::Unknown},
        {GLFW_KEY_SPACE, input::keyboard::Space},
        {GLFW_KEY_APOSTROPHE, input::keyboard::Apostrophe},
        {GLFW_KEY_COMMA, input::keyboard::Comma},
        {GLFW_KEY_MINUS, input::keyboard::Minus},
        {GLFW_KEY_PERIOD, input::keyboard::Period},
        {GLFW_KEY_SLASH, input::keyboard::Slash},
        {GLFW_KEY_0, input::keyboard::Num0},
        {GLFW_KEY_1, input::keyboard::Num1},
        {GLFW_KEY_2, input::keyboard::Num2},
        {GLFW_KEY_3, input::keyboard::Num3},
        {GLFW_KEY_4, input::keyboard::Num4},
        {GLFW_KEY_5, input::keyboard::Num5},
        {GLFW_KEY_6, input::keyboard::Num6},
        {GLFW_KEY_7, input::keyboard::Num7},
        {GLFW_KEY_8, input::keyboard::Num8},
        {GLFW_KEY_9, input::keyboard::Num9},
        {GLFW_KEY_SEMICOLON, input::keyboard::Semicolon},
        {GLFW_KEY_EQUAL, input::keyboard::Equal},
        {GLFW_KEY_A, input::keyboard::A},
        {GLFW_KEY_B, input::keyboard::B},
        {GLFW_KEY_C, input::keyboard::C},
        {GLFW_KEY_D, input::keyboard::D},
        {GLFW_KEY_E, input::keyboard::E},
        {GLFW_KEY_F, input::keyboard::F},
        {GLFW_KEY_G, input::keyboard::G},
        {GLFW_KEY_H, input::keyboard::H},
        {GLFW_KEY_I, input::keyboard::I},
        {GLFW_KEY_J, input::keyboard::J},
        {GLFW_KEY_K, input::keyboard::K},
        {GLFW_KEY_L, input::keyboard::L},
        {GLFW_KEY_M, input::keyboard::M},
        {GLFW_KEY_N, input::keyboard::N},
        {GLFW_KEY_O, input::keyboard::O},
        {GLFW_KEY_P, input::keyboard::P},
        {GLFW_KEY_Q, input::keyboard::Q},
        {GLFW_KEY_R, input::keyboard::R},
        {GLFW_KEY_S, input::keyboard::S},
        {GLFW_KEY_T, input::keyboard::T},
        {GLFW_KEY_U, input::keyboard::U},
        {GLFW_KEY_V, input::keyboard::V},
        {GLFW_KEY_W, input::keyboard::W},
        {GLFW_KEY_X, input::keyboard::X},
        {GLFW_KEY_Y, input::keyboard::Y},
        {GLFW_KEY_Z, input::keyboard::Z},
        {GLFW_KEY_LEFT_BRACKET, input::keyboard::LeftBracket},
        {GLFW_KEY_BACKSLASH, input::keyboard::Backslash},
        {GLFW_KEY_RIGHT_BRACKET, input::keyboard::RightBracket},
        {GLFW_KEY_GRAVE_ACCENT, input::keyboard::GraveAccent},
        {GLFW_KEY_ESCAPE, input::keyboard::Escape},
        {GLFW_KEY_ENTER, input::keyboard::Enter},
        {GLFW_KEY_TAB, input::keyboard::Tab},
        {GLFW_KEY_BACKSPACE, input::keyboard::Backspace},
        {GLFW_KEY_INSERT, input::keyboard::Insert},
        {GLFW_KEY_DELETE, input::keyboard::Delete},
        {GLFW_KEY_RIGHT, input::keyboard::ArrowRight},
        {GLFW_KEY_LEFT, input::keyboard::ArrowLeft},
        {GLFW_KEY_DOWN, input::keyboard::ArrowDown},
        {GLFW_KEY_UP, input::keyboard::ArrowUp},
        {GLFW_KEY_PAGE_UP, input::keyboard::PageUp},
        {GLFW_KEY_PAGE_DOWN, input::keyboard::PageDown},
        {GLFW_KEY_HOME, input::keyboard::Home},
        {GLFW_KEY_END, input::keyboard::End},
        {GLFW_KEY_CAPS_LOCK, input::keyboard::CapsLock},
        {GLFW_KEY_SCROLL_LOCK, input::keyboard::ScrollLock},
        {GLFW_KEY_NUM_LOCK, input::keyboard::NumLock},
        {GLFW_KEY_PRINT_SCREEN, input::keyboard::PrintScreen},
        {GLFW_KEY_PAUSE, input::keyboard::Pause},
        {GLFW_KEY_F1, input::keyboard::F1},
        {GLFW_KEY_F2, input::keyboard::F2},
        {GLFW_KEY_F3, input::keyboard::F3},
        {GLFW_KEY_F4, input::keyboard::F4},
        {GLFW_KEY_F5, input::keyboard::F5},
        {GLFW_KEY_F6, input::keyboard::F6},
        {GLFW_KEY_F7, input::keyboard::F7},
        {GLFW_KEY_F8, input::keyboard::F8},
        {GLFW_KEY_F9, input::keyboard::F9},
        {GLFW_KEY_F10, input::keyboard::F10},
        {GLFW_KEY_F11, input::keyboard::F11},
        {GLFW_KEY_F12, input::keyboard::F12},
        {GLFW_KEY_KP_0, input::keyboard::NumPad0},
        {GLFW_KEY_KP_1, input::keyboard::NumPad1},
        {GLFW_KEY_KP_2, input::keyboard::NumPad2},
        {GLFW_KEY_KP_3, input::keyboard::NumPad3},
        {GLFW_KEY_KP_4, input::keyboard::NumPad4},
        {GLFW_KEY_KP_5, input::keyboard::NumPad5},
        {GLFW_KEY_KP_6, input::keyboard::NumPad6},
        {GLFW_KEY_KP_7, input::keyboard::NumPad7},
        {GLFW_KEY_KP_8, input::keyboard::NumPad8},
        {GLFW_KEY_KP_9, input::keyboard::NumPad9},
        {GLFW_KEY_KP_DECIMAL, input::keyboard::NumPadDecimal},
        {GLFW_KEY_KP_DIVIDE, input::keyboard::NumPadDivide},
        {GLFW_KEY_KP_MULTIPLY, input::keyboard::NumPadMultiply},
        {GLFW_KEY_KP_SUBTRACT, input::keyboard::NumPadSubtract},
        {GLFW_KEY_KP_ADD, input::keyboard::NumPadAdd},
        {GLFW_KEY_KP_ENTER, input::keyboard::NumPadEnter},
        {GLFW_KEY_KP_EQUAL, input::keyboard::NumPadEqual},
        {GLFW_KEY_LEFT_SHIFT, input::keyboard::LeftShift},
        {GLFW_KEY_LEFT_CONTROL, input::keyboard::LeftControl},
        {GLFW_KEY_LEFT_ALT, input::keyboard::LeftAlt},
        {GLFW_KEY_LEFT_SUPER, input::keyboard::LeftSuper},
        {GLFW_KEY_RIGHT_SHIFT, input::keyboard::RightShift},
        {GLFW_KEY_RIGHT_CONTROL, input::keyboard::RightControl},
        {GLFW_KEY_RIGHT_ALT, input::keyboard::RightAlt},
        {GLFW_KEY_RIGHT_SUPER, input::keyboard::RightSuper},
        {GLFW_KEY_MENU, input::keyboard::Menu},
    };

    return glfwKeyMap;
}

const std::unordered_map<input::keyboard, int> input::getKeyboardKeyMap() {
    static const std::unordered_map<input::keyboard, int> map = []() {
        std::unordered_map<input::keyboard, int> result;
        for (const auto& [glfw, kb] : getGlfwKeyMap()) {
            result[kb] = glfw;
        }

        return result;
    }();

    return map;
};