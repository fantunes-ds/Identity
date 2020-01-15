#include <stdafx.h>
#include <Keyboard.h>

using namespace Engine::Input;

bool Keyboard::IsKeyDown(unsigned char p_keycode) noexcept
{
    if (!(m_inputBuffer.find(p_keycode) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_keycode)->second == Event::Type::PRESSED)
        {
            m_inputBuffer.at(p_keycode) = Event::Type::INVALID;
            return true;
        }
    return false;
}

bool Keyboard::IsKeyUp(unsigned char p_keycode) noexcept
{
    if (!(m_inputBuffer.find(p_keycode) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_keycode)->second == Event::Type::RELEASED)
        {
            m_inputBuffer.at(p_keycode) = Event::Type::INVALID;
            return true;
        }
    return false;
}

bool Keyboard::IsKeyHeld(unsigned char p_keycode) noexcept
{
    if (!(m_inputBuffer.find(p_keycode) == m_inputBuffer.end()))
        if (m_inputBuffer.find(p_keycode)->second == Event::Type::PRESSED)
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
    m_inputBuffer.insert_or_assign(p_keycode,Event::Type::PRESSED);
}

void Keyboard::OnKeyReleased(const unsigned char p_keycode) noexcept
{
    m_inputBuffer.insert_or_assign(p_keycode,Event::Type::RELEASED);
}

void Keyboard::OnChar(const unsigned char p_char) noexcept
{
    m_charBuffer.push(p_char);
}

void Keyboard::ClearStates() noexcept
{
    for (auto& key : m_inputBuffer)
        key.second = Event::Type::INVALID;
}

