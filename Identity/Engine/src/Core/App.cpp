#define DEBUG_MODE true;

#include <stdafx.h>

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

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window"), m_width(800), m_height(600)
{
    btVector3 vec(0.0f, 0.0f, 0.0f);

    Input::Input::InitInput();
}

App::App(int p_width, int p_height, const char* p_name, const bool p_isEditor) : m_window(p_width, p_height, p_name),
                                                                                 m_width(p_width), m_height(p_height),
                                                                                 m_isEditor(p_isEditor)
{
    Input::Input::InitInput();
}

void App::Init()
{
    Containers::EventContainer::AddEvent("OnGUI");
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
    camera.GetTransform()->Translate(Vector3F{0.0f, -5.0f, -10.0f});
    camera.AddComponent<Components::Camera>(m_width, m_height);
    Systems::RenderSystem::SetActiveCamera(camera.FindComponentOfType<Components::Camera>()->GetID());
    //----------

    InitEditor();

    float fixedUpdateTimer = 0.0f;
    Systems::PhysicsSystem::FixedUpdate();
    while (true)
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
        if (ImGui::Begin("Hierarchy"))
        {
            int i = 0;
            for (auto& node : Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetRootSceneNodes())
                DisplayNextChild(node.second, i);
        }
        ImGui::End();


        float deltaTime = Tools::Time::GetDeltaTime();

        //Systems
        Systems::PhysicsSystem::Update(deltaTime);
        Systems::TransformSystem::Update(deltaTime);
        Systems::CameraSystem::Update(deltaTime);

        fixedUpdateTimer += deltaTime;
        //todo this should never go below 0
        //update could be at 0.01f
        if (fixedUpdateTimer >= 0.01f || fixedUpdateTimer < 0)
        {
            if (RunBullet)
                Systems::PhysicsSystem::FixedUpdate();
            fixedUpdateTimer = 0.0f;
        }

        TestingSimulation();
        DoFrame(deltaTime);
        EndFrame();
        Tools::Time::Stop();
    }
}

std::shared_ptr<Engine::Scene::SceneNode> App::DisplayNextChild(std::shared_ptr<Scene::SceneNode> p_child, int& p_i)
{
    static ImGuiTreeNodeFlags base_flags         = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int                selection_mask     = (1 << 2);
    int                       node_clicked       = -1;
    ImGuiTreeNodeFlags        node_flags         = base_flags;
    static bool               test_drag_and_drop = true;
    const bool                is_selected        = (selection_mask & (1 << p_i)) != 0;
    if (is_selected)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    if (!p_child->GetChildren().empty())
    {
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)p_i, node_flags, "%s", p_child->GetName().c_str());
        {
            if (ImGui::IsItemClicked())
                node_clicked = p_i;
            if (test_drag_and_drop && ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("%s", p_child->GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if (node_open)
            {
                for (auto& child : p_child->GetChildren())
                    DisplayNextChild(child, p_i);
                ImGui::TreePop();
            }
            p_i++;
        }
    }
    else
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)p_i, node_flags, "%s", p_child->GetName().c_str());
        if (ImGui::IsItemClicked())
            node_clicked = p_i;
        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("%s", p_child->GetName().c_str());
            ImGui::EndDragDropSource();
        }
        p_i++;
    }
    if (node_clicked != -1)
    {
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        else if (!(selection_mask & (1 << node_clicked)))
            selection_mask = (1 << node_clicked);           // Click to single-select
    }

    return p_child;
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

void App::InitEditor()
{
    auto scene = std::make_shared<Scene::Scene>();
    scene->SetName("scene1");
    Managers::SceneManager::AddScene(scene);
    Managers::SceneManager::SetActiveScene(scene);

    auto link  = std::make_shared<Objects::GameObject>("link");
    auto lambo = std::make_shared<Objects::GameObject>("lambo");
    auto light = std::make_shared<Objects::GameObject>("light");

    //---LINK---
    link->GetTransform()->Translate(Vector3F{4.0f, -5.0f, -4.0f});
    link->GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    link->GetTransform()->RotateWithEulerAngles(Vector3F{0.02f, -45.0f, 0.02f});

    link->AddComponent<Components::BoxCollider>();
    link->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{0.5f, 1.0f, 0.5f});
    Vector3F linkOffset{0.0f, -1.0f, 0.0f};
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
    lambo->GetTransform()->Translate(Vector3F{5.0f, 5.0f, -3.0f});
    lambo->GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    lambo->GetTransform()->RotateWithEulerAngles(Vector3F{0.0f, -40.0f, 30.0f});

    lambo->AddComponent<Components::BoxCollider>();
    lambo->FindComponentOfType<Components::BoxCollider>()->SetMass(80);
    lambo->FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{2.0f, 1.0f, 5.0f});
    Vector3F lamboOffset{0.0f, -1.5f, 0.0f};
    lambo->FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(lamboOffset);

    lambo->AddComponent<Components::ModelComponent>("Lambo");
    for (auto& mesh : lambo->GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LamboMat"));
    }
    scene->AddGameObject(lambo);
    //-----------

    //---LIGHT---
    light->GetTransform()->Translate(Vector3F{10.0f, 4.0f, -10.0f});
    light->GetTransform()->Scale(Vector3F{0.01f, 0.01f, 0.01f});

    Rendering::Lights::DirectionalLight::LightData dirLight
    {
        Vector4F(light->GetTransform()->GetPosition().x * -1, light->GetTransform()->GetPosition().y,
                 light->GetTransform()->GetPosition().z * -1, 1.0f),
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
    else if (_INPUT->keyboard.IsKeyDown('O') && RunBullet)
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
