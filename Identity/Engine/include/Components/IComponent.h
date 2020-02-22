#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Components
{
    class API_ENGINE IComponent : public Objects::IObject
    {
    public:

        virtual bool operator==(IComponent* p_other) = 0;

    public:
        virtual bool DeleteFromMemory() = 0;
    };
}
