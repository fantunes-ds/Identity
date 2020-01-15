#include <stdafx.h>
#include <System.h>

using namespace Engine;

System::System() : m_window(800, 600, "Engine Window")
{
    Run();
}

System::~System()
{
}

System::System(int p_width, int p_height, const char* p_name) : m_window(p_width, p_height, p_name)
{
    Run();
}

void System::Run()
{
    while ((m_gResult = GetMessage(&m_msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
        DoFrame();
    }
}

int System::Exit() const
{
    if (m_gResult == -1)
    {
        return -1;
    }

    return m_msg.wParam;
}

void System::DoFrame()
{
    m_window.GetGraphics().ClearBuffer(0.0f, 0.0f, 1.0f);
    m_window.GetGraphics().EndFrame();
}
