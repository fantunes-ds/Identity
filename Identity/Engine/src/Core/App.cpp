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
#include <Components/CameraComponent.h>
#include <Containers/MaterialContainer.h>
#include <Components/LightComponent.h>
#include <LinearMath/btVector3.h>
#include "Components/BoxCollider.h"
#include "Containers/ColliderContainer.h"

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window"), m_width(800), m_height(600)
{
    btVector3 vec(0.0f, 0.0f, 0.0f);
    //btDiscreteDynamicsWorld w;
    //b3PhysicsClientHandle kPhysClient = 0;
    Input::Input::InitInput();
    //btVector3 vec3;
}

App::App(int p_width, int p_height, const char* p_name) : m_window(p_width, p_height, p_name), m_width(p_width), m_height(p_height)
{
    Input::Input::InitInput();
}

int App::Run() const
{
    Systems::RenderSystem renderSystem;
    Tools::FPSCounter fpsCounter(20);

    Objects::GameObject link("link");
    Objects::GameObject lambo("lambo");
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

    link.GetTransform()->Translate(Vector3F{4.0f, -5.0f, -4.0f});
    link.GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    link.GetTransform()->RotateWithEulerAngles(Vector3F{0.02f, -45.0f, 0.02f});
    link.AddComponent<Components::ModelComponent>("../Engine/Resources/YoungLink.obj", "statue");
    link.AddComponent<Components::BoxCollider>();
    link.FindComponentOfType<Components::BoxCollider>()->SetDimensions(GPM::Vector3F{ 0.5f, 1.0f, 0.5f });
    GPM::Vector3F linkOffset{ 0.0f, -1.0f, 0.0f };
    link.FindComponentOfType<Components::BoxCollider>()->SetPositionOffset(linkOffset);
    link.FindComponentOfType<Components::BoxCollider>()->SetName("LinkCollider");

    lambo.GetTransform()->Translate(Vector3F{5.0f, 5.0f, -4.0f});
    lambo.GetTransform()->Scale(Vector3F{ 0.02f, 0.02f, 0.02f });
    //lambo.GetTransform()->RotateWithEulerAngles(Vector3F{ 45.f, 45.f, 90.f });
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

    camera.AddComponent<Components::CameraComponent>(m_width, m_height);
    light.AddComponent<Components::LightComponent>(dirLight);

    for (auto& mesh : link.GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Containers::MaterialContainer::FindMaterial("LinkTexture"));
    }

    for (auto& mesh : lambo.GetModel()->GetMeshes())
    {
        mesh->SetMaterial(Containers::MaterialContainer::FindMaterial("LamboTexture"));
    }

    renderSystem.SetActiveCamera(camera.FindComponentOfType<Components::CameraComponent>()->GetCamera()->GetID());


      lambo.GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -40.0f, 30.0f });

    while (true)
    {
        fpsCounter.Start();

        link.GetTransform()->RotateWithEulerAngles(Vector3F{ 0.0f, -0.0f, 0.02f });
        Containers::ColliderContainer::Update(fpsCounter.GetDeltaTime());
        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }

        DoFrame(renderSystem, fpsCounter.GetDeltaTime());

        fpsCounter.Stop();
    }
}

void App::DoFrame(Engine::Systems::RenderSystem& p_renderSystem, float p_deltaTime) const
{
    Rendering::Renderer::GetInstance()->ClearBuffers(0.3f, 0.3f, 0.3f);

    if (_INPUT->keyboard.IsKeyDown('F'))
        Rendering::Renderer::GetInstance()->SetFullscreen(!Rendering::Renderer::GetInstance()->GetFullscreenState());

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    p_renderSystem.Update(p_deltaTime);

    static bool show_demo_window = true;
    ImGui::Begin("Identity UI Tools");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Rendering::Renderer::GetInstance()->EndFrame();
}
