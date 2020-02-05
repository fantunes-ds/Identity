#pragma once
#include <Export.h>

namespace Engine::Components
{
    class API_ENGINE IComponent
    {
    public:
        virtual bool IsWellInitialized() = 0;
        virtual bool operator==(IComponent* p_other) = 0;
    };
}
