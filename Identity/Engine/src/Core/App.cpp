#define DEBUG_MODE true;

#include <stdafx.h>

#include <Core/App.h>

#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <Tools/Time.h>

#include <Systems/RenderSystem.h>

#include <Input/Input.h>
#include <Objects/GameObject.h>
#include <Components/ModelComponent.h>
#include <Components/Camera.h>
#include <UI/Dockspace.h>

#include <Components/Light.h>
#include <LinearMath/btVector3.h>
#include <Systems/CameraSystem.h>
#include <Systems/TransformSystem.h>

#include <Components/BoxCollider.h>
#include <Systems/PhysicsSystem.h>
#include <Scene/Scene.h>
#include <Managers/SceneManager.h>

#include <Managers/ResourceManager.h>
#include "Containers/GameObjectContainer.h"

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window"), m_width(800), m_height(600)
{
    btVector3 vec(0.0f, 0.0f, 0.0f);

    Input::Input::InitInput();
}

App::App(int p_width, int p_height, const char* p_name, const bool p_isEditor) : m_window(p_width, p_height, p_name), m_width(p_width), m_height(p_height), m_isEditor(p_isEditor)
{
    Input::Input::InitInput();
}

int App::Run()
{
    Managers::ResourceManager::AddTexture("../Engine/Resources/link.png", "LinkText");
    Managers::ResourceManager::AddTexture("../Engine/Resources/lambo_text.jpeg", "LamboText");
    Managers::ResourceManager::CreateMaterial("LinkMat", "defaultPS", "defaultVS", "LinkText");
    Managers::ResourceManager::CreateMaterial("LamboMat", "defaultPS", "defaultVS", "LamboText");

    Managers::ResourceManager::AddModel("../Engine/Resources/YoungLink.obj", "Link");
    Managers::ResourceManager::AddModel("../Engine/Resources/Lambo.obj", "Lambo");

    //--CAMERA--
    Objects::GameObject camera;
    camera.GetTransform()->Translate(Vector3F{ 0.0f, -5.0f, -10.0f });
    camera.AddComponent<Components::Camera>(m_width, m_height);
    Systems::RenderSystem::SetActiveCamera(camera.FindComponentOfType<Components::Camera>()->GetID());
    //----------

    InitScene();

    // auto link = std::make_shared<Objects::GameObject>("link");
    // auto lambo = std::make_shared<Objects::GameObject>("lambo");
    // Objects::GameObject camera;
    // Objects::GameObject light;
    //
    //
    //
    // //---LINK---
    // link->GetTransform()->Translate(Vector3F{4.0f, -5.0f, -4.0f});
    // link->GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    // link->GetTransform()->RotateWithEulerAngles(Vector3F{0.02f, -45.0f, 0.02f});
    //
    // link->AddComponent<Components::BoxCollider>();
    // link->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 0.5f, 1.0f, 0.5f });
    // GPM::Vector3F linkOffset{ 0.0f, -1.0f, 0.0f };
    // link->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(linkOffset);
    // link->FindComponentOfType<Components::BoxCollider>()->SetName("LinkCollider");
    // link->FindComponentOfType<Components::BoxCollider>()->SetActive(false);
    //
    // link->AddComponent<Components::ModelComponent>("Link");
    // for (auto& mesh : link->GetModel()->GetMeshes())
    // {
    //     mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LinkMat"));
    // }
    // scene->AddGameObject(link);
    // //----------
    //
    // //---LAMBO---
    // lambo->GetTransform()->Translate(Vector3F{5.0f, 5.0f, -3.0f});
    // lambo->GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    // lambo->GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -40.0f, 30.0f });
    //
    // lambo->AddComponent<Components::BoxCollider>();
    // lambo->FindComponentOfType<Components::BoxCollider>()->SetMass(80);
    // lambo->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 2.0f, 1.0f, 5.0f });
    // GPM::Vector3F lamboOffset{ 0.0f, -1.5f, 0.0f };
    // lambo->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(lamboOffset);
    //
    // lambo->AddComponent<Components::ModelComponent>("Lambo");
    // for (auto& mesh : lambo->GetModel()->GetMeshes())
    // {
    //     mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LamboMat"));
    // }
    // scene2->AddGameObject(lambo);
    // //-----------
    //
    // //--CAMERA--
    // camera.GetTransform()->Translate(Vector3F{ 0.0f, -5.0f, -10.0f });
    // camera.AddComponent<Components::Camera>(m_width, m_height);
    // //----------
    //
    // //---LIGHT---
    // light.GetTransform()->Translate(Vector3F{ 10.0f, 4.0f, -10.0f });
    // light.GetTransform()->Scale(Vector3F{ 0.01f, 0.01f, 0.01f });
    //
    // Rendering::Lights::DirectionalLight::LightData dirLight
    // {
    // Vector4F(light.GetTransform()->GetPosition().x * -1, light.GetTransform()->GetPosition().y, light.GetTransform()->GetPosition().z * -1, 1.0f),
    // Vector4F(0.1f, 0.1f, 0.1f, 1.0f),
    // Vector4F(1.0f, 1.0f, 0.95f, 1.0f),
    // Vector4F(0.5f, 0.5f, 0.5f, 1.0f),
    // Vector4F(1.0f, 1.0f, 1.0f, 1.0f),
    // 32.0f
    // };
    //
    // light.AddComponent<Components::Light>(dirLight);
    // //-----------
    //
    // Systems::RenderSystem::SetActiveCamera(camera.FindComponentOfType<Components::Camera>()->GetID());
    //
    // // lambo->GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -40.0f, 30.0f });

    float fixedUpdateTimer = 0.0f;
    while (true)
    {
        Tools::Time::Start();
        StartFrame();

        if (m_isEditor)
            Engine::UI::Dockspace::CreateDockspace();

        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }

        float deltaTime = Tools::Time::GetDeltaTime();

        Containers::PhysicsSystem::Update(deltaTime);
        Containers::TransformSystem::Update(deltaTime);
        Containers::CameraSystem::Update(deltaTime);

        fixedUpdateTimer += deltaTime;
        //todo this should never go below 0
        //update could be at 0.01f
        if (fixedUpdateTimer >= 0.01f || fixedUpdateTimer < 0)
        {
            if (RunBullet)
                Containers::PhysicsSystem::FixedUpdate();
            fixedUpdateTimer = 0.0f;
        }

        TestingSimulation();

        DoFrame(deltaTime);
        EndFrame();
        Tools::Time::Stop();
    }
}

void App::StartFrame() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void App::DoFrame(float p_deltaTime) const
{
    Rendering::Renderer::GetInstance()->ClearBuffers(0.3f, 0.3f, 0.3f);

    if (_INPUT->keyboard.IsKeyDown('F'))
        Rendering::Renderer::GetInstance()->SetFullscreen(!Rendering::Renderer::GetInstance()->GetFullscreenState());

    Systems::RenderSystem::GetInstance()->IUpdate(p_deltaTime, m_isEditor);
}

void App::EndFrame() const
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;

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

void App::InitScene(bool play)
{
    if (play)
    {
        InitPlay();
    }
    else
    {
        InitEditor();
    }
}


void App::InitPlay()
{
    auto scene = std::make_shared<Scene::Scene>();
    scene->SetName("scene2");
    Managers::SceneManager::AddScene(scene);
    Managers::SceneManager::SetPlayScene(scene);

    auto link2 = std::make_shared<Objects::GameObject>("link");
    auto lambo2 = std::make_shared<Objects::GameObject>("lambo");
    auto light2 = std::make_shared<Objects::GameObject>("light");

    //---LINK---
    link2->GetTransform()->Translate(Vector3F{ 4.0f, 0.0f, -4.0f });
    link2->GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    link2->GetTransform()->RotateWithEulerAngles(Vector3F{ 0.02f, -45.0f, 0.02f });

    link2->AddComponent<Components::BoxCollider>();
    link2->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 0.5f, 1.0f, 0.5f });
    GPM::Vector3F linkOffset{ 0.0f, -1.0f, 0.0f };
    link2->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(linkOffset);
    link2->FindComponentOfType<Components::BoxCollider>()->SetName("LinkCollider");
    link2->FindComponentOfType<Components::BoxCollider>()->SetActive(true);

    link2->AddComponent<Components::ModelComponent>("Link");
    for (auto& mesh : link2->GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LinkMat"));
    }
    scene->AddGameObject(link2);
    //----------

    //---LAMBO---
    lambo2->GetTransform()->Translate(Vector3F{ 5.0f, 5.0f, -3.0f });
    lambo2->GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    lambo2->GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -40.0f, 30.0f });

    lambo2->AddComponent<Components::BoxCollider>();
    lambo2->FindComponentOfType<Components::BoxCollider>()->SetMass(80);
    lambo2->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 2.0f, 1.0f, 5.0f });
    GPM::Vector3F lamboOffset{ 0.0f, -1.5f, 0.0f };
    lambo2->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(lamboOffset);

    lambo2->AddComponent<Components::ModelComponent>("Lambo");
    for (auto& mesh : lambo2->GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LamboMat"));
    }
    scene->AddGameObject(lambo2);
    //-----------

    //---LIGHT---
    light2->GetTransform()->Translate(Vector3F{ 10.0f, 4.0f, -10.0f });
    light2->GetTransform()->Scale(Vector3F{ 0.01f, 0.01f, 0.01f });

    Rendering::Lights::DirectionalLight::LightData dirLight
    {
    Vector4F(light2->GetTransform()->GetPosition().x * -1, light2->GetTransform()->GetPosition().y, light2->GetTransform()->GetPosition().z * -1, 1.0f),
    Vector4F(0.1f, 0.1f, 0.1f, 1.0f),
    Vector4F(1.0f, 1.0f, 0.95f, 1.0f),
    Vector4F(0.5f, 0.5f, 0.5f, 1.0f),
    Vector4F(1.0f, 1.0f, 1.0f, 1.0f),
    32.0f
    };

    light2->AddComponent<Components::Light>(dirLight);
    scene->AddGameObject(light2);
    //-----------
}

void App::InitEditor()
{
    auto scene = std::make_shared<Scene::Scene>();
    scene->SetName("scene1");
    Managers::SceneManager::AddScene(scene);
    Managers::SceneManager::SetActiveScene(scene);

    auto link = std::make_shared<Objects::GameObject>("link");
    auto lambo = std::make_shared<Objects::GameObject>("lambo");
    auto light = std::make_shared<Objects::GameObject>("light");

    //---LINK---
    link->GetTransform()->Translate(Vector3F{ 4.0f, -5.0f, -4.0f });
    link->GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    link->GetTransform()->RotateWithEulerAngles(Vector3F{ 0.02f, -45.0f, 0.02f });

    link->AddComponent<Components::BoxCollider>();
    link->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 0.5f, 1.0f, 0.5f });
    GPM::Vector3F linkOffset{ 0.0f, -1.0f, 0.0f };
    link->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(linkOffset);
    link->FindComponentOfType<Components::BoxCollider>()->SetName("LinkCollider");

    link->AddComponent<Components::ModelComponent>("Link");
    for (auto& mesh : link->GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LinkMat"));
    }
    scene->AddGameObject(link);
    //----------

    //---LAMBO---
    lambo->GetTransform()->Translate(Vector3F{ 5.0f, 5.0f, -3.0f });
    lambo->GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    lambo->GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -40.0f, 30.0f });

    lambo->AddComponent<Components::BoxCollider>();
    lambo->FindComponentOfType<Components::BoxCollider>()->SetMass(80);
    lambo->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 2.0f, 1.0f, 5.0f });
    GPM::Vector3F lamboOffset{ 0.0f, -1.5f, 0.0f };
    lambo->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(lamboOffset);

    lambo->AddComponent<Components::ModelComponent>("Lambo");
    for (auto& mesh : lambo->GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LamboMat"));
    }
    scene->AddGameObject(lambo);
    //-----------

    //---LIGHT---
    light->GetTransform()->Translate(Vector3F{ 10.0f, 4.0f, -10.0f });
    light->GetTransform()->Scale(Vector3F{ 0.01f, 0.01f, 0.01f });

    Rendering::Lights::DirectionalLight::LightData dirLight
    {
    Vector4F(light->GetTransform()->GetPosition().x * -1, light->GetTransform()->GetPosition().y, light->GetTransform()->GetPosition().z * -1, 1.0f),
    Vector4F(0.1f, 0.1f, 0.1f, 1.0f),
    Vector4F(1.0f, 1.0f, 0.95f, 1.0f),
    Vector4F(0.5f, 0.5f, 0.5f, 1.0f),
    Vector4F(1.0f, 1.0f, 1.0f, 1.0f),
    32.0f
    };

    light->AddComponent<Components::Light>(dirLight);
    scene->AddGameObject(light);
    //-----------
}

void App::TestingSimulation()
{
    if (_INPUT->keyboard.IsKeyDown('P') && !RunBullet)
    {
        if (!Managers::SceneManager::GetPlayScene())
            InitScene(true);

        auto active = Managers::SceneManager::GetActiveScene();

        //deactivate editor scene
        active->SetActiveOnAll(false);

        Managers::SceneManager::SetActiveScene(Managers::SceneManager::GetPlayScene());
        Managers::SceneManager::SetPlayScene(active);
        RunBullet = true;
    }
    else if (_INPUT->keyboard.IsKeyDown('O') && RunBullet)
    {
        auto active = Managers::SceneManager::GetActiveScene();
        Managers::SceneManager::SetActiveScene(Managers::SceneManager::GetPlayScene());
        Managers::SceneManager::SetPlayScene(active);
        Managers::SceneManager::GetActiveScene()->SetActiveOnAll(true);

        //TODO delete old play scene


        RunBullet = false;
    }
}
