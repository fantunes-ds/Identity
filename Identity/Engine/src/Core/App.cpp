#include <stdafx.h>
#include <Core/App.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <Events/IEventCallback.h>
#include "Events/Event.h"

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
    static float angle = 0;

    m_window.GetGraphics().ClearBuffer(1.0f, 1.0f, 1.0f);

    if (m_window.keyboard.IsKeyHeld('R'))
        m_window.GetGraphics().ClearBuffer(1.0f, 0.0f, 0.0f);

    if (m_window.keyboard.IsKeyHeld('G'))
        m_window.GetGraphics().ClearBuffer(0.0f, 1.0f, 0.0f);

    if (m_window.keyboard.IsKeyHeld('B'))
        m_window.GetGraphics().ClearBuffer(0.0f, 0.0f, 1.0f);

    m_window.GetGraphics().DrawLoadedCube("../Engine/Resources/statue.obj");
    // m_window.GetGraphics().DrawTriangle(angle);

    m_window.GetGraphics().EndFrame();

    angle += 0.01;
}
