#pragma once
#include <Export.h>

namespace Engine::Systems
{
    class API_ENGINE IECSSystem
    {
    public:
        virtual void Update(float p_deltaTime) = 0;
    };
}