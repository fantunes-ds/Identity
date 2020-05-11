#define DEBUG_MODE true;

#include <stdafx.h>
#include <fstream>

#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <LinearMath/btVector3.h>

#include <Core/App.h>
#include <Components/Camera.h>
#include <Components/ModelComponent.h>
#include <Components/Light.h>
#include <Components/BoxCollider.h>
#include <Input/Input.h>
#include <Managers/SceneManager.h>
#include <Managers/ResourceManager.h>
#include <Objects/GameObject.h>
#include <Tools/Time.h>
#include <Rendering/Renderer.h>
#include <Scene/Scene.h>
#include <Systems/RenderSystem.h>
#include <Systems/CameraSystem.h>
#include <Systems/TransformSystem.h>
#include <Systems/PhysicsSystem.h>
#include <Systems/LightSystem.h>

Engine::Core::App::App() : m_window(800, 600, "Engine Window"), m_width(800), m_height(600)
{
    btVector3 vec(0.0f, 0.0f, 0.0f);

    Input::Input::InitInput();
}

Engine::Core::App::App(int p_width, int p_height, const char* p_name, const bool p_isEditor) : m_window(p_width, p_height, p_name),
                                                                                 m_width(p_width), m_height(p_height),
                                                                                 m_isEditor(p_isEditor)
{
    Input::Input::InitInput();
}

void Engine::Core::App::Init()
{
    Containers::EventContainer::AddEvent("OnGUI");
}

int Engine::Core::App::Run()
{
    m_applicationIsRunning = true;
    Managers::ResourceManager::Deserialize();

    //--CAMERA--
    Objects::GameObject camera;
    camera.GetTransform()->Translate(Vector3F{0.0f, -5.0f, -10.0f});
    camera.AddComponent<Components::Camera>(m_width, m_height);
    Systems::RenderSystem::SetActiveCamera(camera.FindComponentOfType<Components::Camera>()->GetID());
    //----------

    Managers::SceneManager::LoadScene("scene2.txt");

    float fixedUpdateTimer = 0.0f;
    Systems::PhysicsSystem::FixedUpdate();
    while (m_applicationIsRunning)
    {
        Tools::Time::Start();
        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }
        StartFrame();

        // Events
        // (will be moved below DoFrame once we get rid of all ImGUI calls on Engine)
        Containers::EventContainer::GetEvent("OnGUI").Fire();

        static bool show_demo_window = true;

        static char buffer[1024];

        if (show_demo_window)
        {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        float deltaTime = Tools::Time::GetDeltaTime();

        //Systems
        Systems::PhysicsSystem::Update(deltaTime);
        Systems::TransformSystem::Update(deltaTime);
        Systems::LightSystem::Update(deltaTime);
        Systems::CameraSystem::Update(deltaTime);

        fixedUpdateTimer += deltaTime;
        //todo this should never go below 0
        //update could be at 0.01f
        if (fixedUpdateTimer >= 0.01f)// || fixedUpdateTimer < 0)
        {
            if (RunBullet)
                Systems::PhysicsSystem::FixedUpdate();
            fixedUpdateTimer = 0.0f;
        }

        DoFrame(deltaTime);
        EndFrame();
        Tools::Time::Stop();
    }

    Managers::ResourceManager::Serialize();
}

void Engine::Core::App::StartFrame() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Engine::Core::App::DoFrame(float p_deltaTime) const
{
    Rendering::Renderer::GetInstance()->ClearBuffers(0.3f, 0.3f, 0.3f);

    if (_INPUT->keyboard.IsKeyDown('F'))
        Rendering::Renderer::GetInstance()->SetFullscreen(!Rendering::Renderer::GetInstance()->GetFullscreenState());

    Systems::RenderSystem::GetInstance()->IUpdate(p_deltaTime, m_isEditor);
}

void Engine::Core::App::EndFrame() const
{
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::Begin("Identity UI Tools");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Rendering::Renderer::GetInstance()->EndFrame();
}


//TODO this should be in scene manager
void Engine::Core::App::TestingSimulation()
{
    if (!RunBullet)
    {
        // if (!Managers::SceneManager::GetPlayScene())
        // InitScene(true);

        auto playScene = std::make_shared<Scene::Scene>();
        playScene->SetName("scene2");
        auto activeScene = Managers::SceneManager::GetActiveScene();
        Managers::SceneManager::DuplicateScene(playScene, activeScene);
        Managers::SceneManager::AddScene(playScene);
        Managers::SceneManager::SetActiveScene(playScene);
        Managers::SceneManager::SetPlayScene(activeScene);
        Managers::SceneManager::GetActiveScene()->SetActiveOnAll(true);
        Managers::SceneManager::GetPlayScene()->SetActiveOnAll(false);

        //deactivate editor scene

        // Managers::SceneManager::SetActiveScene(Managers::SceneManager::GetPlayScene());
        // Managers::SceneManager::SetPlayScene(activeScene);
        RunBullet = true;
    }
    else if (RunBullet)
    {
        auto active = Managers::SceneManager::GetActiveScene();
        Managers::SceneManager::SetActiveScene(Managers::SceneManager::GetPlayScene());
        Managers::SceneManager::SetPlayScene(active);
        Managers::SceneManager::GetPlayScene()->SetActiveOnAll(false);
        Managers::SceneManager::GetActiveScene()->SetActiveOnAll(true);

        //TODO delete old play scene
        Managers::SceneManager::DeletePlayScene();

        RunBullet = false;
    }
}
