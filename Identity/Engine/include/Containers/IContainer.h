#pragma once
#include <Export.h>

namespace Engine::Containers
{
    class API_ENGINE IContainer
    {
    public:
        template <class T, typename>
        void RemoveElement();
    };
}
