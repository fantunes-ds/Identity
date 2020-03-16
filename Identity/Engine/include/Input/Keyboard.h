#pragma once
#include <Export.h>
#include <queue>
#include <unordered_map>
#include <WinSetup.h>

// Forward declaration for friending the class later-on

namespace Engine::Rendering
{
    class Window;
}

/**
 * This namespace is responsible for any types of input
 */
namespace Engine::Input
{
    /**
     * @brief The class responsible for Keyboard actions
     */
    class API_ENGINE Keyboard
    {
   /**
    * Friending class Window so it can access private functions and members and give input
    */
    friend class Rendering::Window;

    public:
        /**
         * @brief The different states the Keys can have
         */
        enum KeyState
        {
            PRESSED = 0,
            RELEASED = 1,
            INVALID = 2
        };

        // We exceptionally don't use CAPS for KeyCode, as it has codes that
        // already exist in Windows and would cause compatibility issues.

        /**
         * @brief The map to the physical Key on the keyboard
         */
        enum KeyCode
        {
            Invalid = -1, //not assigned to our keyboard (shouldn't be sent as return of a key)
            Backspace = VK_BACK,
            Delete = VK_DELETE,
            Tab = VK_TAB,
            Return = VK_RETURN,
            Escape = VK_ESCAPE,
            Space = VK_SPACE,
            KeyPad0 = VK_NUMPAD0,
            KeyPad1 = VK_NUMPAD1,
            KeyPad2 = VK_NUMPAD2,
            KeyPad3 = VK_NUMPAD3,
            KeyPad4 = VK_NUMPAD4,
            KeyPad5 = VK_NUMPAD5,
            KeyPad6 = VK_NUMPAD6,
            KeyPad7 = VK_NUMPAD7,
            KeyPad8 = VK_NUMPAD8,
            KeyPad9 = VK_NUMPAD9,
            KeyPadPeriod = 110,
            KeyPadDivide = VK_DIVIDE,
            KeyPadMultiply = VK_MULTIPLY,
            KeyPadMinus = 109,
            KeyPadPlus = 102,
            KeyPadEnter = VK_RETURN,
            UpArrow = VK_UP,
            DownArrow = VK_DOWN,
            RightArrow = VK_RIGHT,
            LeftArrow = VK_LEFT,
            Insert = VK_INSERT,
            Home = VK_HOME,
            End = VK_END,
            PageUp = 33,
            PageDown = 34,
            F1 = VK_F1,
            F2 = VK_F2,
            F3 = VK_F3,
            F4 = VK_F4,
            F5 = VK_F5,
            F6 = VK_F6,
            F7 = VK_F7,
            F8 = VK_F8,
            F9 = VK_F9,
            F10 = VK_F10,
            F11 = VK_F11,
            F12 = VK_F12,
            F13 = VK_F13,
            F14 = VK_F14,
            F15 = VK_F15,
            Alpha0 = 48,
            Alpha1 = 49,
            Alpha2 = 50,
            Alpha3 = 51,
            Alpha4 = 52,
            Alpha5 = 53,
            Alpha6 = 54,
            Alpha7 = 55,
            Alpha8 = 56,
            Alpha9 = 57,
            Backslash = 220,
            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,
            Tilde = VK_OEM_3,
            Numlock = VK_NUMLOCK,
            CapsLock = VK_CAPITAL,
            ScrollLock = VK_SCROLL,
            RightShift = 16,
            LeftShift = 16,
            RightControl = 17,
            LeftControl = 17,
            RightAlt = 18,
            LeftAlt = 18,
            LeftWindows = VK_LWIN,
            RightWindows = VK_RWIN,
            Print = VK_PRINT,
            Mouse0 = VK_LBUTTON,
            Mouse1 = VK_RBUTTON,
            Mouse2 = VK_MBUTTON,
        };

        Keyboard() = default;
        ~Keyboard() = default;

        Keyboard(const Keyboard&) = delete;
        Keyboard(const Keyboard&&) = delete;

        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(const Keyboard&&) = delete;

        /**
         * @brief Detects if a key has been pressed at a certain frame
         * @return true once next frame if key is down.
         * @param p_charInCapital : The character you want to check the status of.
         * It NEEDS to be in capital letter, as lowcase will not be checked/detected.
         * @warning Only works in an update loop. You need to be able to detect at every frame
         */
        [[nodiscard]] bool IsKeyDown(const unsigned char p_charInCapital) noexcept;
        /**
         * @brief Detects if a key has been pressed at a certain frame
         * @return true once next frame if key is down.
         * @param p_keyCode : The KeyCode enum parameter you want to check.
         * @warning Only works in an update loop. You need to be able to detect at every frame
         */
        [[nodiscard]] bool IsKeyDown(const KeyCode p_keyCode) noexcept;
        /**
         * @brief Detects if a key has been released at a certain frame
         * @return true once next frame if key is released.
         * @param p_charInCapital : The character you want to check the status of.
         * It NEEDS to be in capital letter, as lowcase will not be checked/detected.
         * @warning Only works in an update loop. You need to be able to detect at every frame
         */
        [[nodiscard]] bool IsKeyUp(const unsigned char p_charInCapital) noexcept;
        /**
         * @brief Detects if a key has been released at a certain frame
         * @return true once next frame if key is released.
         * @param p_keyCode : The KeyCode enum parameter you want to check.
         * @warning Only works in an update loop. You need to be able to detect at every frame
         */
        [[nodiscard]] bool IsKeyUp(const KeyCode p_keyCode) noexcept;
        /**
         * @brief Detects if a key is being pressed
         * @return true next frames as long as key is pressed.
         * @param p_charInCapital : The character you want to check the status of.
         * It NEEDS to be in capital letter, as lowcase will not be checked/detected.
         * @warning Only works in an update loop. You need to be able to detect at every frame
         */
        [[nodiscard]] bool IsKeyHeld(const unsigned char p_charInCapital) noexcept;
        /**
         * @brief Detects if a key is being pressed
         * @return true next frames as long as key is pressed.
         * @param p_keyCode : The KeyCode enum parameter you want to check.
         * @warning Only works in an update loop. You need to be able to detect at every frame
         */
        [[nodiscard]] bool IsKeyHeld(const KeyCode p_keyCode) noexcept;

        // char events, will be used to retrieve text input
        char ReadChar() noexcept;
        [[nodiscard]] bool IsCharEmpty() const noexcept;
        void FlushChar() noexcept;

        // windows auto-repeat control
        void EnableAutoRepeat() noexcept;
        void DisableAutoRepeat() noexcept;
        [[nodiscard]] bool IsAutoRepeatEnabled() const noexcept;
    private:
        /**
         * @brief Event to be triggered when a key is pressed
         * @param p_keycode : The key that was pressed
         */
        void OnKeyPressed(const unsigned char p_keycode) noexcept;
        /**
         * @brief Event to be triggered when a key is released
         * @param p_keycode : The key that was released
         */
        void OnKeyReleased(const unsigned char p_keycode) noexcept;
        /**
         * @brief Event to be triggered when a key is pressed if you want
         * to have the name of the key pressed, and not just update it's state.
         * @param p_char : The key that was pressed
         */
        void OnChar(const char unsigned p_char) noexcept;

        /**
         * @brief This function resets default Invalid state for all keys in the map
         */
        void ClearStates() noexcept;

        bool m_autoRepeat = false;
        std::unordered_map<int, int> m_inputBuffer;
        std::queue<char> m_charBuffer;
    };
}
