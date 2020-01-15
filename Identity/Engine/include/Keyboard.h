#pragma once
#include <Export.h>
#include <queue>
#include <unordered_map>

namespace Engine::Renderer
{
    class Window;
}

namespace Engine::Input
{
    class API_ENGINE Keyboard
    {
    friend class Renderer::Window;

    public:
        enum KeyState
        {
            PRESSED = 0,
            RELEASED = 1,
            INVALID = 2
        };
    private:
        KeyState m_keyState;

    public:
        Keyboard() = default;
        ~Keyboard() = default;

        Keyboard(const Keyboard&) = delete;
        Keyboard(const Keyboard&&) = delete;

        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(const Keyboard&&) = delete;

        // key events
        [[nodiscard]] bool IsKeyDown(unsigned char p_keycode) noexcept;
        [[nodiscard]] bool IsKeyUp(unsigned char p_keycode) noexcept;
        [[nodiscard]] bool IsKeyHeld(unsigned char p_keycode) noexcept;

        // char events
        char ReadChar() noexcept;
        [[nodiscard]] bool IsCharEmpty() const noexcept;
        void FlushChar() noexcept;

        // auto-repeat control
        void EnableAutoRepeat() noexcept;
        void DisableAutoRepeat() noexcept;
        [[nodiscard]] bool IsAutoRepeatEnabled() const noexcept;
    private:
        void OnKeyPressed(unsigned char p_keycode) noexcept;
        void OnKeyReleased(unsigned char p_keycode) noexcept;
        void OnChar(char unsigned p_char) noexcept;

        void ClearStates() noexcept;

        bool m_autoRepeat = false;
        std::unordered_map<int, int> m_inputBuffer;
        std::queue<char> m_charBuffer;
    };
}
