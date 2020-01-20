#include <stdafx.h>
#include <Input/Mouse.h>

using namespace Engine::Input;

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return m_mouseMap.second;
}

int Mouse::GetPosX() const noexcept
{
    return m_mouseMap.second.first;
}

int Mouse::GetPosY() const noexcept
{
    return m_mouseMap.second.second;
}

bool Mouse::IsInWindow() const noexcept
{
    return IsInWindow();
}

bool Mouse::LeftIsPressed() const noexcept
{
    return m_leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
    return m_rightIsPressed;
}

void Mouse::Flush() noexcept
{
    m_mouseMap = std::pair<MouseState, std::pair<int, int>>();
}

void Mouse::OnMouseMove(const int p_x, const int p_y) noexcept
{
    m_x = p_x;
    m_y = p_y;

    m_mouseMap.first = MOVE;
    m_mouseMap.second = std::make_pair(m_x, m_y);
}

void Mouse::OnLeftPressed() noexcept
{
    m_leftIsPressed = true;

    m_mouseMap.first = L_PRESSED;
}

void Mouse::OnLeftReleased() noexcept
{
    m_leftIsPressed = false;

    m_mouseMap.first = L_RELEASED;
}

void Mouse::OnRightPressed() noexcept
{
    m_rightIsPressed = true;

    m_mouseMap.first = R_PRESSED;
}

void Mouse::OnRightReleased() noexcept
{
    m_rightIsPressed = false;

    m_mouseMap.first = R_RELEASED;
}

void Mouse::OnWheelUp() noexcept
{
    m_mouseMap.first = WHEEL_UP;
}

void Mouse::OnWheelDown() noexcept
{
    m_mouseMap.first = WHEEL_DOWN;
}