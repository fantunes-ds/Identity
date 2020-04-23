#pragma once
#include <Export.h>

namespace Engine::Systems
{
    class API_ENGINE ISystem
    {
    public:
        virtual void IUpdate(const float p_deltaTime, bool p_isEditor = false) = 0;
    };
}