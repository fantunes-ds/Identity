#pragma once
#include <Export.h>
#include <Rendering/Window.h>

namespace Engine::Core
{
    class API_ENGINE System
    {
    public:
        System();
        ~System();
        System(int p_width, int p_height, const char* p_name);

        void Run();

        int Exit() const;

    private:
        Rendering::Window m_window;
        MSG m_msg;
        BOOL m_gResult;

    };
}
