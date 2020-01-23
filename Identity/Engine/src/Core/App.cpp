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
    GPM::Vector3D pos{ 0.0, 0.0, 0.0 };
    GPM::Vector3D scale{ 1.0, 1.0, 1.0 };
    GPM::Vector3D rot{ 90, 0, 0};

    ObjectElements::Transform transform(pos, scale, rot);
    //transform.RotateWithEulerAngles({ -90, 0, 0 });
    OutputDebugString(transform.GetForward().ToString().c_str());
    OutputDebugString(transform.GetRight().ToString().c_str());


    auto* callback = new EventCallback<ObjectElements::Transform>(&transform, &ObjectElements::Transform::bullshit);
     Event something;
     something.AddListener(callback);
     something.Fire();
    OutputDebugString(transform.GetForward().ToString().c_str());

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

    m_window.GetGraphics().DrawLoadedCube("../Engine/Resources/cube.obj");
    // m_window.GetGraphics().DrawTriangle(angle);

    m_window.GetGraphics().EndFrame();

    if (m_window.keyboard.IsKeyDown(Input::Keyboard::D))
        OutputDebugString("D was pressed\n");
    else if (m_window.keyboard.IsKeyUp(Input::Keyboard::U))
        OutputDebugString("U was released\n");
    else if (m_window.keyboard.IsKeyHeld(Input::Keyboard::H))
        OutputDebugString("H was held\n");

    if (m_window.mouse.GetState() == Input::Mouse::MOVE)
    {
        std::string output = std::to_string(m_window.mouse.GetPosX()) + '\n';
        OutputDebugString(output.c_str());
    }
    else if (m_window.mouse.GetState() == Input::Mouse::LEAVE)
        m_window.SetTitle("Left screen\n");

    angle += 0.01;
}
