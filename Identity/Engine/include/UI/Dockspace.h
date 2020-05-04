#pragma once

#include <Export.h>

namespace Engine::UI
{
    class API_ENGINE Dockspace
    {
    public:
        Dockspace() = delete;
        ~Dockspace() = delete;

        static void CreateDockspace();
    };
}