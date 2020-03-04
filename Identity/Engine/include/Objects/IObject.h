#pragma once
#include <Export.h>

namespace Engine::Objects
{
    class API_ENGINE IObject
    {
    public:
        IObject();
        virtual ~IObject() = default;

        [[nodiscard]] int32_t GetID() const { return m_id; }
        [[nodiscard]] const std::string& GetName() const { return m_name; }
        void SetName(const std::string& p_name) { m_name = p_name; }

    protected:
        int32_t m_id = -1;
        std::string m_name = "NoName";
    };
}
