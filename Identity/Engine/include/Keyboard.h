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
        class API_ENGINE Event
        {
        public:
            enum Type
            {
                PRESSED = 0,
                RELEASED = 1,
                INVALID = 2
            };
        private:
            Type m_type;
            unsigned char m_code;
        public:
            Event() noexcept
                :
                m_type(INVALID),
                m_code(0u)
            {}

            Event(const Type p_type, const unsigned char p_code) noexcept
                :
                m_type(p_type),
                m_code(p_code)
            {}

            [[nodiscard]] bool IsPressed() const noexcept
            {
                return m_type == PRESSED;
            }

            [[nodiscard]] bool IsReleased() const noexcept
            {
                return m_type == RELEASED;
            }

            [[nodiscard]] bool IsValid() const noexcept
            {
                return m_type != INVALID;
            }

            [[nodiscard]] unsigned char GetCode() const noexcept
            {
                return m_code;
            }
        };

        //redifining public for visibility only.
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
