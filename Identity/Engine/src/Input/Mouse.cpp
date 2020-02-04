#include <stdafx.h>
#include <Input/Mouse.h>

using namespace Engine::Input;

Mouse::MouseState Mouse::GetState() const noexcept
{
    return m_mouseMap.first;
}

Vector2I Mouse::GetPos() const noexcept
{
    return m_mouseMap.second;
}

int Mouse::GetPosX() const noexcept
{
    return m_mouseMap.second.x;
}

int Mouse::GetPosY() const noexcept
{
    return m_mouseMap.second.y;
}

bool Mouse::IsInWindow() const noexcept
{
    return m_isInWindow;
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
    m_mouseMap = std::pair<MouseState, Vector2I>();
}

void Mouse::OnMouseMove(const int p_x, const int p_y) noexcept
{
    m_x = p_x;
    m_y = p_y;

    m_mouseMap.first = MOVE;
    m_mouseMap.second = Vector2I(m_x,m_y);
}

void Mouse::OnMouseEnter() noexcept
{
    m_isInWindow = true;
    m_mouseMap.first = ENTER;
}

void Mouse::OnMouseLeave() noexcept
{
    m_isInWindow = false;
    m_mouseMap.first = LEAVE;
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