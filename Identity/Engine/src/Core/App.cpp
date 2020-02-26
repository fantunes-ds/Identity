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

    // Containers::ModelContainer::AddModel("../Engine/Resources/YoungLink.obj", "statue");
    // Containers::ModelContainer::AddModel("../Engine/Resources/Lambo.obj", "lambo");

    camera.AddComponent<Components::CameraComponent>(m_width, m_height);


    Containers::LightContainer* test = Containers::LightContainer::GetInstance();

    gameObject.GetTransform()->Translate(Vector3F{3.0f, 0.0f, 4.0f});
    gameObject.GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    gameObject.AddComponent<Components::ModelComponent>("../Engine/Resources/YoungLink.obj", "statue");

    gameObject2.GetTransform()->Translate(Vector3F{6.0f, 0.0f, -4.0f});
    gameObject2.GetTransform()->Scale(Vector3F{0.02f, 0.02f, 0.02f});
    gameObject2.AddComponent<Components::ModelComponent>("../Engine/Resources/Lambo.obj", "lambo");


    light.GetTransform()->Translate(Vector3F{10.0f, 4.0f, -10.0f});
    light.GetTransform()->Scale(Vector3F{0.1f, 0.1f, 0.1f});

    Rendering::Lights::Light::LightData dirLight;

    dirLight.position  = Vector4F(light.GetTransform()->GetPosition().x * -1, light.GetTransform()->GetPosition().y, light.GetTransform()->GetPosition().z * -1, 1.0f);
    dirLight.ambient   = Vector4F(0.1f, 0.1f, 0.1f, 1.0f);
    dirLight.diffuse   = Vector4F(1.0f, 1.0f, 0.95f, 1.0f);
    dirLight.specular  = Vector4F(0.5f, 0.5f ,0.5f, 1.0f);
    dirLight.color     = Vector4F(1.0f, 1.0f, 1.0f, 1.0f);
    dirLight.shininess = 32.0f;

    int32_t cameraComponentID = camera.AddComponent<Components::CameraComponent>(m_width, m_height);
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
    Rendering::Renderer::GetInstance()->ClearBuffer(0.3f, 0.3f, 0.3f);
    if (_INPUT->keyboard.IsKeyHeld('R'))
        Rendering::Renderer::GetInstance()->ClearBuffer(1.0f, 0.0f, 0.0f);

    if (_INPUT->keyboard.IsKeyHeld('G'))
        Rendering::Renderer::GetInstance()->ClearBuffer(0.0f, 1.0f, 0.0f);

    if (_INPUT->keyboard.IsKeyHeld('B'))
        Rendering::Renderer::GetInstance()->ClearBuffer(0.0f, 0.0f, 1.0f);

    if (_INPUT->keyboard.IsKeyDown('F'))
        Rendering::Renderer::GetInstance()->SetFullscreen(!Rendering::Renderer::GetInstance()->GetFullscreenState());

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    p_renderSystem.Update();

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
