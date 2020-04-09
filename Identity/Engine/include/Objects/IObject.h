#pragma once
#include <Export.h>

namespace Engine::Objects
{
    /**
     * @brief Class inherited by any class that needs an ID and/or a name to be easily identified.
     */
    class API_ENGINE IObject
    {
    public:
        IObject();
        virtual ~IObject() = default;

        [[nodiscard]] int32_t GetID() const { return m_id; }
        [[nodiscard]] inline const std::string& GetName() const { return m_name; }
        inline void SetName(const std::string& p_name) { m_name = p_name; }

    protected:
        int32_t m_id = -1;
        std::string m_name = "NoName";
    };
}
