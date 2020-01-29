#pragma once
#include <Export.h>
#include <Rendering/Window.h>

#include <Tools/ImGUI/ImGUIManager.h>
#include <3DLoader/Manager/ModelManager.h>

namespace Engine::Core
{
    /*
     @brief When creating a new app, create it followed by the "run" method
     By default the window size is 800x600 and can be change when creating your app
     */
    class API_ENGINE App
    {
    public:
        App();
        ~App() = default;
        App(int p_width, int p_height, const char* p_name);
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        int Run();

    private:
        void DoFrame();

    private:
        Tools::UI::ImGUIManager m_imguiManager;
        Rendering::Window m_window;
        MSG m_msg;
        BOOL m_gResult;
    };
}
