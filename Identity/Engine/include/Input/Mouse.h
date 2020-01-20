#pragma once
#include <queue>
#include <unordered_map>


namespace Engine::Rendering
{
    class Window;
}

namespace Engine::Input
{
    class Mouse
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
            INVALID = 8
        };

        Mouse() = default;
        Mouse(const Mouse&) = delete;
        Mouse& operator=(const Mouse&) = delete;


        [[nodiscard]] std::pair<int, int> GetPos() const noexcept;
        [[nodiscard]] int GetPosX() const noexcept;
        [[nodiscard]] int GetPosY() const noexcept;
        bool IsInWindow() const noexcept;
        bool LeftIsPressed() const noexcept;
        bool RightIsPressed() const noexcept;

        void Flush() noexcept;
    private:
        void OnMouseMove(int p_x, int p_y) noexcept;
        void OnLeftPressed() noexcept;
        void OnLeftReleased() noexcept;
        void OnRightPressed() noexcept;
        void OnRightReleased() noexcept;
        void OnWheelUp() noexcept;
        void OnWheelDown() noexcept;
    private:
        static constexpr unsigned int buffer_size = 16u;
        int                           m_x{0};
        int                           m_y{0};
        bool                          m_leftIsPressed = false;
        bool                          m_rightIsPressed = false;
        std::pair<MouseState, std::pair<int, int>> m_mouseMap;
    };
}
