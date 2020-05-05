#pragma once
#include <Export.h>
#include <Rendering/Window.h>

#include <Tools/ImGUI/ImGUIManager.h>

#include <Scene/SceneGraph/SceneNode.h>
#include <Systems/RenderSystem.h>

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

        int Run();
        void Init();
        void TestingSimulation();
        void SetIsApplicationRunning(bool p_state) { m_applicationIsRunning = p_state; }
    private:
        void StartFrame() const;
        void DoFrame(float p_deltaTime) const;
        void EndFrame() const;

        //testing
        void InitEditor();
        bool RunBullet{ false };
        //-------

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
