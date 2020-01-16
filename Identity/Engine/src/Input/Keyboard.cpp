#include <stdafx.h>
#include <Input/Keyboard.h>

using namespace Engine::Input;

bool Keyboard::IsKeyDown(unsigned char p_charInCapital) noexcept
{
    if (!(m_inputBuffer.find(p_charInCapital) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_charInCapital)->second == PRESSED)
        {
            m_inputBuffer.at(p_charInCapital) = INVALID;
            return true;
        }
    return false;
}

bool Keyboard::IsKeyDown(KeyCode p_keyCode) noexcept
{
    if (!(m_inputBuffer.find(p_keyCode) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_keyCode)->second == PRESSED)
        {
            m_inputBuffer.at(p_keyCode) = INVALID;
            return true;
        }
    return false;
}

bool Keyboard::IsKeyUp(unsigned char p_charInCapital) noexcept
{
    if (!(m_inputBuffer.find(p_charInCapital) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_charInCapital)->second == RELEASED)
        {
            m_inputBuffer.at(p_charInCapital) = INVALID;
            return true;
        }
    return false;
}

bool Keyboard::IsKeyUp(KeyCode p_keyCode) noexcept
{
    if (!(m_inputBuffer.find(p_keyCode) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_keyCode)->second == RELEASED)
        {
            m_inputBuffer.at(p_keyCode) = INVALID;
            return true;
        }
    return false;
}

bool Keyboard::IsKeyHeld(unsigned char p_charInCapital) noexcept
{
    if (!(m_inputBuffer.find(p_charInCapital) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_charInCapital)->second == PRESSED)
            return true;
    return false;
}

bool Keyboard::IsKeyHeld(KeyCode p_keyCode) noexcept
{
    if (!(m_inputBuffer.find(p_keyCode) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_keyCode)->second == PRESSED)
            return true;
    return false;
}

char Keyboard::ReadChar() noexcept
{
    if (!m_charBuffer.empty())
    {
        const char charCode = m_charBuffer.front();
        m_charBuffer.pop();
        return charCode;
    }
    return 0;
}

bool Keyboard::IsCharEmpty() const noexcept
{
    return m_charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
    m_charBuffer = std::queue<char>();
}

void Keyboard::EnableAutoRepeat() noexcept
{
    m_autoRepeat = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
    m_autoRepeat = false;
}

bool Keyboard::IsAutoRepeatEnabled() const noexcept
{
    return m_autoRepeat;
}

void Keyboard::OnKeyPressed(const unsigned char p_keycode) noexcept
{
    m_inputBuffer.insert_or_assign(p_keycode, PRESSED);
}

void Keyboard::OnKeyReleased(const unsigned char p_keycode) noexcept
{
    m_inputBuffer.insert_or_assign(p_keycode, RELEASED);
}

void Keyboard::OnChar(const unsigned char p_char) noexcept
{
    m_charBuffer.push(p_char);
}

void Keyboard::ClearStates() noexcept
{
    for (auto& key : m_inputBuffer)
        key.second = INVALID;
}

