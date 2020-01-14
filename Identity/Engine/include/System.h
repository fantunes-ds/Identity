#pragma once
#include <Export.h>
#include <Window.h>

namespace Engine
{
    class API_ENGINE System
    {
    public:
        System();
        ~System() = default;
        System(int p_width, int p_height, const char* p_name);

        void Run();

        int Exit() const;

    private:
        Renderer::Window m_window;
        MSG m_msg;
        BOOL m_gResult;

    };
}
