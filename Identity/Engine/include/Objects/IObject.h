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

    protected:
        int32_t m_id = -1;

    private:
    };
}
