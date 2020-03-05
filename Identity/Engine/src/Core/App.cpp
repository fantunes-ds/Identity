#include <stdafx.h>

#include <Core/App.h>

#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>

#include <Systems/RenderSystem.h>
#include <Input/Input.h>
#include <Objects/GameObject.h>
#include <Components/ModelComponent.h>
#include <Components/CameraComponent.h>
#include <Containers/MaterialContainer.h>
#include <Components/LightComponent.h>

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window"), m_width(800), m_height(600)
{
    Input::Input::InitInput();
}

App::App(int p_width, int p_height, const char* p_name) : m_window(p_width, p_height, p_name), m_width(p_width), m_height(p_height)
{
    Input::Input::InitInput();
}

int App::Run() const
{
    Systems::RenderSystem renderSystem;
    Objects::GameObject gameObject;
    Objects::GameObject gameObject2;
    Objects::GameObject camera;
    Objects::GameObject light;

    Containers::MaterialContainer::AddMaterial("missing");
    Containers::MaterialContainer::GetMaterial("missing")->AddTexture(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/missing.png");
    Containers::MaterialContainer::GetMaterial("missing")->AddPixelShader(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/Shaders/PixelShader.cso");
    Containers::MaterialContainer::GetMaterial("missing")->AddVertexShader(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/Shaders/VertexShader.cso");

    Containers::MaterialContainer::AddMaterial("LinkTexture");
    Containers::MaterialContainer::GetMaterial("LinkTexture")->AddTexture(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/link.png");
    Containers::MaterialContainer::GetMaterial("LinkTexture")->AddPixelShader(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/Shaders/PixelShader.cso");
    Containers::MaterialContainer::GetMaterial("LinkTexture")->AddVertexShader(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/Shaders/VertexShader.cso");

    Containers::MaterialContainer::AddMaterial("LamboTexture");
    Containers::MaterialContainer::GetMaterial("LamboTexture")->AddTexture(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/lambo_text.jpeg");
    Containers::MaterialContainer::GetMaterial("LamboTexture")->AddPixelShader(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/Shaders/PixelShader.cso");
    Containers::MaterialContainer::GetMaterial("LamboTexture")->AddVertexShader(Rendering::Renderer::GetInstance()->GetDevice(), L"../Engine/Resources/Shaders/VertexShader.cso");

    camera.AddComponent<Components::CameraComponent>(m_width, m_height);

    Containers::LightContainer* test = Containers::LightContainer::GetInstance();

    gameObject.GetTransform()->Translate(Vector3F{3.0f, -5.0f, 4.0f});
    gameObject.GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    gameObject.AddComponent<Components::ModelComponent>("../Engine/Resources/YoungLink.obj", "statue");

    gameObject2.GetTransform()->Translate(Vector3F{6.0f, 5.0f, -4.0f});
    gameObject2.GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    gameObject2.GetTransform()->RotateWithEulerAngles(Vector3F{ 45.f, 45.f, 90.f });
    gameObject2.AddComponent<Components::ModelComponent>("../Engine/Resources/Lambo.obj", "lambo");

    camera.GetTransform()->Translate(Vector3F{ 0.0f, 0.0f, -10.0f });

    light.GetTransform()->Translate(Vector3F{10.0f, 4.0f, -10.0f});
    light.GetTransform()->Scale(Vector3F{0.1f, 0.1f, 0.1f});

    Rendering::Lights::Light::LightData dirLight;

    dirLight.position  = Vector4F(light.GetTransform()->GetPosition().x * -1, light.GetTransform()->GetPosition().y, light.GetTransform()->GetPosition().z * -1, 1.0f);
    dirLight.ambient   = Vector4F(0.1f, 0.1f, 0.1f, 1.0f);
    dirLight.diffuse   = Vector4F(1.0f, 1.0f, 0.95f, 1.0f);
    dirLight.specular  = Vector4F(0.5f, 0.5f ,0.5f, 1.0f);
    dirLight.color     = Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
    dirLight.shininess = 32.0f;

    camera.AddComponent<Components::CameraComponent>(m_width, m_height);
    // gameObject.AddComponent<Components::ModelComponent>("../Engine/Resources/statue.obj", "statue");
    // gameObject2.AddComponent<Components::ModelComponent>("../Engine/Resources/Box.fbx", "cube");
    light.AddComponent<Components::ModelComponent>("../Engine/Resources/Box.fbx", "cube");
    light.AddComponent<Components::LightComponent>(dirLight);

    for (auto& mesh : gameObject.GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Containers::MaterialContainer::FindMaterial("LinkTexture"));
    }

    for (auto& mesh : gameObject2.GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Containers::MaterialContainer::FindMaterial("LamboTexture"));
    }

    renderSystem.SetActiveCamera(camera.FindComponentOfType<Components::CameraComponent>()->GetCamera()->GetID());

    while (true)
    {
        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }

        DoFrame(renderSystem);
    }
}

void App::DoFrame(Engine::Systems::RenderSystem& p_renderSystem) const
{
    Rendering::Renderer::GetInstance()->ClearBuffers(0.3f, 0.3f, 0.3f);

    if (_INPUT->keyboard.IsKeyDown('F'))
        Rendering::Renderer::GetInstance()->SetFullscreen(!Rendering::Renderer::GetInstance()->GetFullscreenState());

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    //static bool opt_fullscreen_persistant = true;
    //bool opt_fullscreen = opt_fullscreen_persistant;
    //static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    //// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    //// because it would be confusing to have two docking targets within each others.
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    //if (opt_fullscreen)
    //{
    //    ImGuiViewport* viewport = ImGui::GetMainViewport();
    //    ImGui::SetNextWindowPos(viewport->Pos);
    //    ImGui::SetNextWindowSize(viewport->Size);
    //    ImGui::SetNextWindowViewport(viewport->ID);
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    //    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    //    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    //    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    //}

    //// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
    //// and handle the pass-thru hole, so we ask Begin() to not render a background.
    //if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    //    window_flags |= ImGuiWindowFlags_NoBackground;
    //static bool test = true;
    //// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    //// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    //// all active windows docked into it will lose their parent and become undocked.
    //// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    //// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    //ImGui::Begin("DockSpace Demo", &test, window_flags);
    //ImGui::PopStyleVar();

    //if (opt_fullscreen)
    //    ImGui::PopStyleVar(2);

    //// DockSpace
    //if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    //{
    //    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    //    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    //    if (ImGui::BeginMenuBar())
    //    {
    //        if (ImGui::BeginMenu("File"))
    //        {
    //            ImGui::MenuItem("Open", "", nullptr);
    //            ImGui::MenuItem("Save", "", nullptr);
    //            ImGui::MenuItem("Exit", "", nullptr);
    //            ImGui::Separator();
    //            ImGui::EndMenu();
 
    //        }
    //        if (ImGui::BeginMenu("Edit"))
    //        {
    //            ImGui::MenuItem("Just Another Menu here", "", nullptr);
    //            ImGui::Separator();
    //            ImGui::EndMenu();
    //        }

    //        ImGui::EndMenuBar();
    //    }
    //}
    //ImGui::End();
    p_renderSystem.Update();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Rendering::Renderer::GetInstance()->EndFrame();
}
