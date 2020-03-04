#pragma once
#include <Export.h>
#include <GPM/GPM.h>
#include <optional>
#include <queue>

namespace Engine::Rendering
{
    class Window;
}

namespace Engine::Input
{
    /**
     * @brief The class responsible for Mouse actions
     */
    class API_ENGINE Mouse
    {
        /**
         * Friending class Window so it can access private functions and members and give input
         */
        friend class Rendering::Window;
    public:
        /**
         *
         * "@brief Struct defined purely for easier readability, represents a 2D position
         */
        struct RawDelta
        {
            int x{ 0 }, y{ 0 };
        };

        /**
         * @brief The different states the Mouse can have
         */
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

        Mouse()                        = default;
        Mouse(const Mouse&)            = delete;
        Mouse& operator=(const Mouse&) = delete;

        /**
         * @brief Gets the state of the mouse at the current Frame
         * @return a State from the enum
         */
        [[nodiscard]] MouseState              GetState() const noexcept;
        /**
         * @brief Gets the position of the Mouse
         * @return The position in X,Y coordinates
         */
        [[nodiscard]] Vector2I                GetPos() const noexcept;
        /**
         * @brief Gets the raw position of the Mouse
         * @return The raw position in X,Y coordinates
         * @warning Value may be 0,0 or bigger than the window
         */
        [[nodiscard]] std::optional<RawDelta> GetRawPosition() noexcept;
        /**
         * @brief Gets the X position of the Mouse in the window
         * @return The x position of the Mouse in the window
         */
        [[nodiscard]] int                     GetPosX() const noexcept;
        /**
         * @brief Gets the Y position of the Mouse in the window
         * @return The Y position of the Mouse in the window
         */
        [[nodiscard]] int                     GetPosY() const noexcept;
        /**
         * @brief Use this function to know whether or not the Mouse is in the window
         * @return true if it's in the window, false if it isn't
         */
        [[nodiscard]] bool                    IsInWindow() const noexcept;
        /**
         * @brief Use this function to know whether the Left mouse button is pressed or not
         * @return true if left button is pressed, false if it isn't
         */
        [[nodiscard]] bool                    LeftIsPressed() const noexcept;
        /**
         * @brief Use this function to know whether the Right mouse button is pressed or not
         * @return true if right button is pressed, false if it isn't
         */
        [[nodiscard]] bool                    RightIsPressed() const noexcept;

        /**
         * @brief Clears the buffers
         */
        void Flush() noexcept;
        /**
         * @brief Enables usage of Raw Input
         */
        void EnableRawInput() noexcept;
        /**
         * @brief Disables usage of Raw Input
         */
        void DisableRawInput() noexcept;
        /**
         * @brief Get whether Raw Input is enabled or not
         * @return true if Raw Input is enabled, false if it isn't
         */
        [[nodiscard]] bool GetIsRawInputEnabled() const noexcept { return m_isRawInputEnabled; };
    private:
        /**
         * @brief Event to be triggered when the mouse position has changed on the window
         * @param p_x : The x position of the mouse
         * @param p_y : The y position of the mouse
         */
        void OnMouseMove(int p_x, int p_y) noexcept;
        /**
         * @brief Event to be triggered when the mouse enters the window
         */
        void OnMouseEnter() noexcept;
        /**
         * @brief Event to be triggered when the mouse leaves the window
         */
        void OnMouseLeave() noexcept;
        /**
         * @brief Event to be triggered when the mouse left button is pressed
         */
        void OnLeftPressed() noexcept;
        /**
         * @brief Event to be triggered when the mouse left button is released
         */
        void OnLeftReleased() noexcept;
        /**
         * @brief Event to be triggered when the mouse right button is pressed
         */
        void OnRightPressed() noexcept;
        /**
         * @brief Event to be triggered when the mouse right button is released
         */
        void OnRightReleased() noexcept;
        /**
         * @brief Event to be triggered when the mouse wheel goes up
         */
        void OnWheelUp() noexcept;
        /**
         * @brief Event to be triggered when the mouse wheel goes down
         */
        void OnWheelDown() noexcept;
        /**
         * @brief Event to be triggered to know how much the wheel has rolled
         */
        void OnWheelDelta(int p_delta) noexcept;
        /**
         * @brief Event to be triggered to retrieve the raw input of the mouse movement.
         */
        void OnRawDelta(int p_deltaX, int p_deltaY) noexcept;
        /**
         * @brief trims the raw input buffer.
         */
        void TrimRawDeltaBuffer() noexcept;
    private:
        bool                            m_leftIsPressed{false};
        bool                            m_rightIsPressed{false};
        bool                            m_isInWindow{false};
        bool                            m_isRawInputEnabled{false};
        int                             m_x{0};
        int                             m_y{0};
        int                             m_wheelDelta{0};
        static constexpr unsigned int   buffer_size = 16u;
        std::pair<MouseState, Vector2I> m_mouseMap{};
        std::queue<RawDelta>            m_rawDeltaBuffer;
    };
}
