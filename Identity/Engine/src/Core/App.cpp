#define DEBUG_MODE true;

#include <stdafx.h>

#include <Core/App.h>

#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <Tools/FPSCounter.h>

#include <Systems/RenderSystem.h>

#include <Input/Input.h>
#include <Objects/GameObject.h>
#include <Components/ModelComponent.h>
#include <Components/Camera.h>
#include <Containers/MaterialContainer.h>
#include <Components/LightComponent.h>
#include <LinearMath/btVector3.h>
#include <Systems/CameraSystem.h>
#include <Systems/TransformSystem.h>

#include "Components/BoxCollider.h"
#include "Containers/ColliderContainer.h"
#include "Managers/ResourceManager.h"

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window"), m_width(800), m_height(600)
{
    btVector3 vec(0.0f, 0.0f, 0.0f);

    Input::Input::InitInput();
}

App::App(int p_width, int p_height, const char* p_name, bool p_isEditor) : m_window(p_width, p_height, p_name), m_width(p_width), m_height(p_height), m_isEditor(p_isEditor)
{
    Input::Input::InitInput();
}

int App::Run() const
{
    Systems::RenderSystem renderSystem;
    Tools::FPSCounter fpsCounter(200);

    Objects::GameObject link("link");
    Objects::GameObject lambo("lambo");
    Objects::GameObject camera;
    Objects::GameObject light;


    Managers::ResourceManager::AddTexture("../Engine/Resources/link.png", "LinkText");
    Managers::ResourceManager::AddTexture("../Engine/Resources/lambo_text.jpeg", "LamboText");
    Managers::ResourceManager::CreateMaterial("LinkMat", "defaultPS", "defaultVS", "LinkText");
    Managers::ResourceManager::CreateMaterial("LamboMat", "defaultPS", "defaultVS", "LamboText");

    camera.AddComponent<Components::Camera>(m_width, m_height);

    Containers::LightContainer* test = Containers::LightContainer::GetInstance();

    link.GetTransform()->Translate(Vector3F{4.0f, -5.0f, -4.0f});
    link.GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    link.GetTransform()->RotateWithEulerAngles(Vector3F{0.02f, -45.0f, 0.02f});
    link.AddComponent<Components::ModelComponent>("../Engine/Resources/YoungLink.obj", "statue");
    link.AddComponent<Components::BoxCollider>();
    link.FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 0.5f, 1.0f, 0.5f });
    GPM::Vector3F linkOffset{ 0.0f, -1.0f, 0.0f };
    link.FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(linkOffset);
    link.FindComponentOfType<Components::BoxCollider>()->SetName("LinkCollider");

    lambo.GetTransform()->Translate(Vector3F{5.0f, 5.0f, -3.0f});
    lambo.GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    lambo.AddComponent<Components::ModelComponent>("../Engine/Resources/Lambo.obj", "lambo");
    lambo.AddComponent<Components::BoxCollider>();
    lambo.FindComponentOfType<Components::BoxCollider>()->SetMass(1);
    lambo.FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 2.0f, 1.0f, 5.0f });
    GPM::Vector3F lamboOffset{ 0.0f, -1.5f, 0.0f };
    lambo.FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(lamboOffset);


    camera.GetTransform()->Translate(Vector3F{ 0.0f, -5.0f, -10.0f });

    light.GetTransform()->Translate(Vector3F{ 10.0f, 4.0f, -10.0f });
    light.GetTransform()->Scale(Vector3F{ 0.01f, 0.01f, 0.01f });

    Rendering::Lights::Light::LightData dirLight;

    dirLight.position = Vector4F(light.GetTransform()->GetPosition().x * -1, light.GetTransform()->GetPosition().y, light.GetTransform()->GetPosition().z * -1, 1.0f);
    dirLight.ambient = Vector4F(0.1f, 0.1f, 0.1f, 1.0f);
    dirLight.diffuse = Vector4F(1.0f, 1.0f, 0.95f, 1.0f);
    dirLight.specular = Vector4F(0.5f, 0.5f, 0.5f, 1.0f);
    dirLight.color = Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
    dirLight.shininess = 32.0f;

    light.AddComponent<Components::LightComponent>(dirLight);

    for (auto& mesh : link.GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LinkMat"));
    }

    for (auto& mesh : lambo.GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Managers::ResourceManager::GetMaterial("LamboMat"));
    }

    renderSystem.SetActiveCamera(camera.FindComponentOfType<Components::Camera>()->GetID());

    lambo.GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -40.0f, 30.0f });
    // � ----  � will be removed once we have serialization

    while (true)
    {
        fpsCounter.Start();
        StartFrame();

        //todo move below code to an UI class on Engine and call from Editor.
        if (m_isEditor)
        {

            static bool opt_fullscreen_persistant = true;
            bool opt_fullscreen = opt_fullscreen_persistant;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;
            static bool test = true;
            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &test, window_flags);
            ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            ImGuiIO& io = ImGui::GetIO(); (void)io;


            // DockSpace
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        ImGui::MenuItem("Open", "", nullptr);
                        ImGui::MenuItem("Save", "", nullptr);
                        ImGui::MenuItem("Exit", "", nullptr);
                        ImGui::Separator();
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Edit"))
                    {
                        ImGui::MenuItem("Just Another Menu here", "", nullptr);
                        ImGui::Separator();
                        ImGui::EndMenu();
                    }

                    ImGui::EndMenuBar();
                }
            }
            ImGui::End();

            static bool show_demo_window = true;

            if (show_demo_window)
            {
                ImGui::ShowDemoWindow(&show_demo_window);
            }
        }

        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }
        Containers::ColliderContainer::Update(fpsCounter.GetDeltaTime());
        Containers::TransformSystem::Update(fpsCounter.GetDeltaTime());
        Containers::CameraSystem::Update(fpsCounter.GetDeltaTime());

        Containers::ColliderContainer::Update(fpsCounter.GetDeltaTime());
        Containers::TransformSystem::Update(fpsCounter.GetDeltaTime());
        Containers::CameraSystem::Update(fpsCounter.GetDeltaTime());
        
        DoFrame(renderSystem, fpsCounter.GetDeltaTime());
        EndFrame();
        fpsCounter.Stop();
    }
}

void App::StartFrame() const
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void App::DoFrame(Engine::Systems::RenderSystem& p_renderSystem, float p_deltaTime) const
{
    Rendering::Renderer::GetInstance()->ClearBuffers(0.3f, 0.3f, 0.3f);

    if (_INPUT->keyboard.IsKeyDown('F'))
        Rendering::Renderer::GetInstance()->SetFullscreen(!Rendering::Renderer::GetInstance()->GetFullscreenState());

    p_renderSystem.IUpdate(p_deltaTime, m_isEditor);
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
