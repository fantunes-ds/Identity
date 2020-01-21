#include <stdafx.h>
#include <Core/App.h>

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

    if (m_window.m_keyboard.IsKeyHeld('R'))
        m_window.GetGraphics().ClearBuffer(1.0f, 0.0f, 0.0f);

    if (m_window.m_keyboard.IsKeyHeld('G'))
        m_window.GetGraphics().ClearBuffer(0.0f, 1.0f, 0.0f);

    if (m_window.m_keyboard.IsKeyHeld('B'))
        m_window.GetGraphics().ClearBuffer(0.0f, 0.0f, 1.0f);

    m_window.GetGraphics().DrawTriangle();

    m_window.GetGraphics().EndFrame();

    if (m_window.m_keyboard.IsKeyDown(Input::Keyboard::D))
        OutputDebugString("D was pressed\n");
    else if (m_window.m_keyboard.IsKeyUp(Input::Keyboard::U))
        OutputDebugString("U was released\n");
    else if (m_window.m_keyboard.IsKeyHeld(Input::Keyboard::H))
        OutputDebugString("H was held\n");
}
