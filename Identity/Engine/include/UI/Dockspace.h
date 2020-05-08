#pragma once

#include <Export.h>

namespace Engine::UI
{
    class Dockspace
    {
    public:
        Dockspace() = delete;
        ~Dockspace() = delete;

        API_ENGINE static void CreateDockspace(Core::App& p_appRef);
        static void CreateMenuBar(Core::App& p_appRef);
    };
}