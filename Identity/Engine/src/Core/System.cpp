#include <stdafx.h>
#include <Core/System.h>

using namespace Engine::Core;

System::System() : m_window(800, 600, "Engine Window")
{
}

System::~System()
{
}

System::System(int p_width, int p_height, const char* p_name) : m_window(p_width, p_height, p_name)
{
}

void System::Run()
{
    while ((m_gResult = GetMessage(&m_msg, nullptr, 0, 0)) > 0)
    {
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
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
