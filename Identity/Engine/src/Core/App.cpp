#include <stdafx.h>
#include <Core/App.h>
#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <Events/IEventCallback.h>
#include "Events/Event.h"
#include <Systems/RenderSystem.h>

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window")
{
}

App::App(int p_width, int p_height, const char* p_name) : m_window(p_width, p_height, p_name)
{
}

int App::Run()
{
    Manager::ModelManager::GetInstance()->SetGraphicsDevice(m_window.GetRenderer().m_pDevice);

    Systems::RenderSystem renderSystem(&m_window.GetRenderer());
    renderSystem.AddModel("../Engine/Resources/statue.obj", "statue");

    while (true)
    {
        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }
        
        DoFrame(renderSystem);
    }
}

void App::DoFrame(Engine::Systems::RenderSystem& p_renderSystem)
{
    static float angle = 0;

    m_window.GetRenderer().ClearBuffer(1.0f, 1.0f, 1.0f);

    if (m_window.keyboard.IsKeyHeld('R'))
        m_window.GetRenderer().ClearBuffer(1.0f, 0.0f, 0.0f);

    if (m_window.keyboard.IsKeyHeld('G'))
        m_window.GetRenderer().ClearBuffer(0.0f, 1.0f, 0.0f);

    if (m_window.keyboard.IsKeyHeld('B'))
        m_window.GetRenderer().ClearBuffer(0.0f, 0.0f, 1.0f);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    p_renderSystem.Update();

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


    angle += 0.01;
    m_window.GetRenderer().EndFrame();
}
