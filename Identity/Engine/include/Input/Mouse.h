#pragma once
#include <Export.h>
#include <GPM/GPM.h>

namespace Engine::Rendering
{
    class Window;
}

namespace Engine::Input
{
    class API_ENGINE Mouse
    {
        friend class Rendering::Window;
    public:
        enum MouseState
        {
            L_PRESSED = 0,
            L_RELEASED = 2,
            R_PRESSED = 1,
            R_RELEASED = 3,
            WHEEL_DOWN = 4,
            WHEEL_UP = 5,
            MOVE = 6,
            ENTER = 7,
            LEAVE = 8,
            INVALID = 9
        };

        Mouse() = default;
        Mouse(const Mouse&) = delete;
        Mouse& operator=(const Mouse&) = delete;


        [[nodiscard]] MouseState GetState() const noexcept;
        [[nodiscard]] Vector2I GetPos() const noexcept;
        [[nodiscard]] int GetPosX() const noexcept;
        [[nodiscard]] int GetPosY() const noexcept;
        [[nodiscard]] bool IsInWindow() const noexcept;
        [[nodiscard]] bool LeftIsPressed() const noexcept;
        [[nodiscard]] bool RightIsPressed() const noexcept;

        void Flush() noexcept;
    private:
        void OnMouseMove(int p_x, int p_y) noexcept;
        void OnMouseEnter() noexcept;
        void OnMouseLeave() noexcept;
        void OnLeftPressed() noexcept;
        void OnLeftReleased() noexcept;
        void OnRightPressed() noexcept;
        void OnRightReleased() noexcept;
        void OnWheelUp() noexcept;
        void OnWheelDown() noexcept;
        void OnWheelDelta(int p_delta) noexcept;
    private:
        static constexpr unsigned int   buffer_size = 16u;
        int                             m_x{0};
        int                             m_y{0};
        bool                            m_leftIsPressed{false};
        bool                            m_rightIsPressed{false};
        bool                            m_isInWindow{false};
        int                             m_wheelDelta{0};
        std::pair<MouseState, Vector2I> m_mouseMap;
    };
}
