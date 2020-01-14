#include <stdafx.h>
#include <Keyboard.h>

using namespace Engine::Input;

bool Keyboard::IsKeyDown(unsigned char p_keycode) const noexcept
{
    return m_keyStates[p_keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
    if (!m_keyBuffer.empty())
    {
        const Event e = m_keyBuffer.front();
        m_keyBuffer.pop();
        return e;
    }
    return Event();
}

bool Keyboard::IsKeyEmpty() const noexcept
{
    return m_keyBuffer.empty();
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

void Keyboard::FlushKey() noexcept
{
    m_keyBuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept
{
    m_charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
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
    m_keyStates[p_keycode] = true;
    m_keyBuffer.push(Event(Event::Type::PRESSED, p_keycode));
    TrimBuffer(m_keyBuffer);
}

void Keyboard::OnKeyReleased(const unsigned char p_keycode) noexcept
{
    m_keyStates[p_keycode] = false;
    m_keyBuffer.push(Event(Event::Type::RELEASED, p_keycode));
    TrimBuffer(m_keyBuffer);
}

void Keyboard::OnChar(const unsigned char p_char) noexcept
{
    m_charBuffer.push(p_char);
    TrimBuffer(m_charBuffer);
}

void Keyboard::ClearStates() noexcept
{
    m_keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& p_buffer) noexcept
{
    while (p_buffer.size() > bufferSize)
    {
        p_buffer.pop();
    }
}

