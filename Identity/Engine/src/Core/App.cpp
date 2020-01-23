#include <stdafx.h>
#include <Core/App.h>
#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window")
{
}

App::App(int p_width, int p_height, const char* p_name) : m_window(p_width, p_height, p_name)
{
}

int App::Run()
{
    while (true)
    {
        if (const auto eCode = Rendering::Window::ProcessMessage())
        {
            return *eCode;
        }
        DoFrame();
    }
}

void App::DoFrame()
{
    m_window.GetGraphics().ClearBuffer(1.0f, 1.0f, 1.0f);

    if (m_window.keyboard.IsKeyHeld('R'))
        m_window.GetGraphics().ClearBuffer(1.0f, 0.0f, 0.0f);

    if (m_window.keyboard.IsKeyHeld('G'))
        m_window.GetGraphics().ClearBuffer(0.0f, 1.0f, 0.0f);

    if (m_window.keyboard.IsKeyHeld('B'))
        m_window.GetGraphics().ClearBuffer(0.0f, 0.0f, 1.0f);

    if (m_window.keyboard.IsKeyDown(Input::Keyboard::D))
        OutputDebugString("D was pressed\n");
    else if (m_window.keyboard.IsKeyUp(Input::Keyboard::U))
        OutputDebugString("U was released\n");
    else if (m_window.keyboard.IsKeyHeld(Input::Keyboard::H))
        OutputDebugString("H was held\n");

    m_window.GetGraphics().DrawTriangle();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

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

    m_window.GetGraphics().EndFrame();

}
