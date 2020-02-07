#pragma once
#include <Export.h>

namespace Engine::Components
{
    class API_ENGINE IComponent
    {
    public:
        //Temporary function to make class abstract
        virtual bool IsWellInitialized() = 0;
    };
}
