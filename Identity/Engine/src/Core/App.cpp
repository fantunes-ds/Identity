#define DEBUG_MODE true;

#include <stdafx.h>

#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <LinearMath/btVector3.h>

#include <Core/App.h>
#include <Components/Camera.h>
#include <Components/Light.h>
#include <Components/BoxCollider.h>
#include <Components/SphereCollider.h>
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
#include "Components/Sound.h"
#include "Systems/SoundSystem.h"
#include "UI/Hierarchy.h"

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
    camera.GetTransform()->Translate(Vector3F{ 0.0f, .0f, 0.0f });
    camera.AddComponent<Components::Camera>(m_width, m_height);
    // camera.AddComponent<Components::BoxCollider>();
    // camera.FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    Systems::RenderSystem::SetActiveCamera(camera.FindComponentOfType<Components::Camera>()->GetID());
    //----------

    // Managers::SceneManager::LoadScene("scene1");
    
    auto demoScene = std::make_shared<Scene::Scene>("Demo");

    auto floor = std::make_shared<Objects::GameObject>("Floor");
    floor->GetTransform()->SetPosition({ -15.f, -5.f, 15.f });
    floor->GetTransform()->SetScale({ 30,2,30 });
    floor->AddComponent<Components::BoxCollider>();
    floor->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 15, 1, 15 });
    floor->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 15, 1, -15 });
    floor->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(floor);

    auto steep = std::make_shared<Objects::GameObject>("Steep");
    steep->GetTransform()->SetPosition({ 4,-3,0 });
    Quaternion rot;
    rot.MakeFromEuler({ 20, 0, 0 });
    steep->GetTransform()->SetRotation(rot);
    steep->GetTransform()->SetScale({ 3, 0.2f, 5});
    steep->AddComponent<Components::BoxCollider>();
    steep->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 1.5f, 0.1f, 2.5f });
    steep->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 1.5f, 0.1f, -2.5f });
    steep->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(steep);

    auto ball = std::make_shared<Objects::GameObject>("Ball");
    ball->GetTransform()->SetPosition({ 5, 3, -3 });
    ball->GetTransform()->SetScale({ 0.5f, 0.5f, 0.5f });
    ball->AddComponent<Components::SphereCollider>();
    ball->FindComponentOfType<Components::SphereCollider>()->SetPositionOffset({ 0.25f,0.25f,-0.25f });
    ball->FindComponentOfType<Components::SphereCollider>()->SetRadius(0.5f);
    ball->FindComponentOfType<Components::SphereCollider>()->SetMass(100.0f);
    ball->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(ball);

    auto cube1 = std::make_shared<Objects::GameObject>("Cube1");
    cube1->GetTransform()->SetPosition({ 5.5f, -3, 3 });
    cube1->GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
    cube1->AddComponent<Components::BoxCollider>();
    cube1->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 0.1f, 0.1f, 0.1f });
    cube1->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 0.1f, 0.1f, -0.1f });
    cube1->FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    cube1->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(cube1);

    auto cube2 = std::make_shared<Objects::GameObject>("Cube2");
    cube2->GetTransform()->SetPosition({ 5.3f, -3, 3 });
    cube2->GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
    cube2->AddComponent<Components::BoxCollider>();
    cube2->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 0.1f, 0.1f, 0.1f });
    cube2->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 0.1f, 0.1f, -0.1f });
    cube2->FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    cube2->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(cube2);

    auto cube3 = std::make_shared<Objects::GameObject>("Cube3");
    cube3->GetTransform()->SetPosition({ 5.3f, -2.8f, 3 });
    cube3->GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
    cube3->AddComponent<Components::BoxCollider>();
    cube3->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 0.1f, 0.1f, 0.1f });
    cube3->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 0.1f, 0.1f, -0.1f });
    cube3->FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    cube3->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(cube3);

    auto cube4 = std::make_shared<Objects::GameObject>("Cube4");
    cube4->GetTransform()->SetPosition({ 5.5f, -2.8f, 3 });
    cube4->GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
    cube4->AddComponent<Components::BoxCollider>();
    cube4->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 0.1f, 0.1f, 0.1f });
    cube4->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 0.1f, 0.1f, -0.1f });
    cube4->FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    cube4->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(cube4);

    auto cube5 = std::make_shared<Objects::GameObject>("Cube5");
    cube5->GetTransform()->SetPosition({ 5.3f, -2.6f, 3 });
    cube5->GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
    cube5->AddComponent<Components::BoxCollider>();
    cube5->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 0.1f, 0.1f, 0.1f });
    cube5->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 0.1f, 0.1f, -0.1f });
    cube5->FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    cube5->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(cube5);

    auto cube6 = std::make_shared<Objects::GameObject>("Cube6");
    cube6->GetTransform()->SetPosition({ 5.5f, -2.6f, 3 });
    cube6->GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
    cube6->AddComponent<Components::BoxCollider>();
    cube6->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 0.1f, 0.1f, 0.1f });
    cube6->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 0.1f, 0.1f, -0.1f });
    cube6->FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    cube6->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(cube6);

    // auto cube2 = std::make_shared<Objects::GameObject>("Cube2");
    // cube2->GetTransform()->SetPosition({ -0.5f, -0.5f, 8.f });
    // cube2->AddComponent<Components::ModelComponent>("Cube");
    // lightScene->AddGameObject(cube2);
    //
    // auto cube3 = std::make_shared<Objects::GameObject>("Cube3");
    // cube3->GetTransform()->SetPosition({ -15.f, -5.f, 15.f });
    // cube3->GetTransform()->SetScale({ 30,2,30 });
    // cube3->AddComponent<Components::BoxCollider>();
    // cube3->FindComponentOfType<Components::BoxCollider>()->SetDimensions({ 15, 1, 15 });
    // cube3->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset({ 15, 1, -15 });
    // cube3->AddComponent<Components::ModelComponent>("Cube");
    // lightScene->AddGameObject(cube3);
    
    auto light = std::make_shared<Objects::GameObject>("Light");
    light->GetTransform()->SetPosition({ 1.f, 0.f, 0.f });
    light->GetTransform()->SetScale({ 0.2f, 0.2f, 0.2f });
    Rendering::Lights::DirectionalLight::LightData lightData = {
        {light->GetTransform()->GetPosition().x, light->GetTransform()->GetPosition().y, light->GetTransform()->GetPosition().z, 1.0f},
    {1.f, 1.f, 1.f, 0.1f},
    {1.0f, 1.0f,1.0f, 0.1f},
    {1.f, 0.f, 0.f,0.5f},
        128.f, 500.f, 20.f,0
    };

    //
    light->AddComponent<Components::Light>(lightData);
    light->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(light);
    
    auto light2 = std::make_shared<Objects::GameObject>("Light2");
    light2->GetTransform()->SetPosition({ -1.f, 0.f, 0.f });
    light2->GetTransform()->SetScale({ 0.2f, 0.2f, 0.2f });
    light2->AddComponent<Components::Light>(lightData);
    light2->AddComponent<Components::ModelComponent>("Cube");
    demoScene->AddGameObject(light2);
    
    Managers::SceneManager::AddScene(demoScene);
    Managers::SceneManager::SetActiveScene(demoScene);

    float fixedUpdateTimer = 0.0f;

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
        Systems::SoundSystem::Update(deltaTime);


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
void Engine::Core::App::TestingSimulation(bool p_stop)
{
    //Possible Spaghetti code
    UI::Hierarchy::m_currentlySelected = -1;

    if (!RunBullet && !p_stop)
    {
        // if (!Managers::SceneManager::GetPlayScene())
        // InitScene(true);

        auto playScene = std::make_shared<Scene::Scene>();
        playScene->SetName("play");
        auto activeScene = Managers::SceneManager::GetActiveScene();
        Managers::SceneManager::DuplicateScene(playScene, activeScene);
        Managers::SceneManager::AddScene(playScene);
        Managers::SceneManager::SetActiveScene(playScene);
        Managers::SceneManager::SetPlayScene(activeScene);
        Managers::SceneManager::GetActiveScene()->SetActiveOnAll(true);
        Managers::SceneManager::GetPlayScene()->SetActiveOnAll(false);

        // Systems::RenderSystem::SetActiveCamera(Managers::SceneManager::GetPlayScene()->GetActiveCamera()->GetID());

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
