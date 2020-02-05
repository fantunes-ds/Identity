#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Components
{
    class API_ENGINE IComponent : public Objects::IObject
    {
    public:
        virtual bool IsWellInitialized() = 0;
        virtual bool operator==(IComponent* p_other) = 0;
    };
}
