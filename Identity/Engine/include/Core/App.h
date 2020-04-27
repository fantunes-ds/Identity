#pragma once
#include <Export.h>
#include <Rendering/Window.h>

#include <Tools/ImGUI/ImGUIManager.h>
#include <Containers/ModelContainer.h>

namespace Engine {
    namespace Systems {
        class RenderSystem;
    }
}

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
        App(int p_width, int p_height, const char* p_name, bool p_isEditor = false);
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        int Run() const;

    private:
        void StartFrame() const;
        void DoFrame(Systems::RenderSystem& p_renderSystem, float p_deltaTime) const;
        void EndFrame() const;

    private:
        Tools::UI::ImGUIManager m_imguiManager;
        Rendering::Window m_window;
        MSG m_msg;
        BOOL m_gResult;
        int m_width;
        int m_height;
        bool m_isEditor;
        bool m_applicationIsRunning;
    };
}
