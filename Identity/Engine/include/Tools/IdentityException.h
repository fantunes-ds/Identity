#pragma once

#include <exception>
#include <string>

namespace Engine::Tools
{
    class IdentityException : public std::exception
    {
    public:
        IdentityException(int p_line, const char* p_file) noexcept;
        const char* what() const noexcept override;
        virtual const char* GetType() const noexcept;
        int GetLine() const noexcept;
        const std::string& GetFile() const noexcept;
        std::string GetOriginString() const noexcept;

    private:
        int m_line;
        std::string m_file;

    protected:
        mutable std::string whatBuffer;

    };
}