#include <stdafx.h>

#include <Tools/IdentityException.h>

using namespace Engine::Tools;

IdentityException::IdentityException(int p_line, const char* p_file) noexcept
    : m_line(p_line),
      m_file(p_file)
{}

const char* IdentityException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* IdentityException::GetType() const noexcept
{
    return "Identity Exception";
}

int IdentityException::GetLine() const noexcept
{
    return m_line;
}

std::string IdentityException::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << m_file << std::endl
        << "[Line] " << m_line;
    return oss.str();
}
