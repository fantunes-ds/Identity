#include <stdafx.h>
#include <Core/App.h>
#include <Input/Input.h>

using namespace Engine::Core;

App::App() : m_window(800, 600, "Engine Window")
{
    Input::Input::InitInput();
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

    if (INPUT->keyboard.IsKeyHeld('R'))
        m_window.GetGraphics().ClearBuffer(1.0f, 0.0f, 0.0f);

    if (INPUT->keyboard.IsKeyHeld('G'))
        m_window.GetGraphics().ClearBuffer(0.0f, 1.0f, 0.0f);

    if (INPUT->keyboard.IsKeyHeld('B'))
        m_window.GetGraphics().ClearBuffer(0.0f, 0.0f, 1.0f);

    m_window.GetGraphics().EndFrame();

    if (INPUT->keyboard.IsKeyDown(Input::Keyboard::D))
        OutputDebugString("D was pressed\n");
    else if (INPUT->keyboard.IsKeyUp(Input::Keyboard::U))
        OutputDebugString("U was released\n");
    else if (INPUT->keyboard.IsKeyHeld(Input::Keyboard::H))
        OutputDebugString("H was held\n");

    if (INPUT->mouse.GetState() == Input::Mouse::MOVE)
    {
        std::string output = std::to_string(INPUT->mouse.GetPosX()) + '\n';
        OutputDebugString(output.c_str());
    }
    else if (INPUT->mouse.GetState() == Input::Mouse::LEAVE)
        m_window.SetTitle("Left screen\n");
}
