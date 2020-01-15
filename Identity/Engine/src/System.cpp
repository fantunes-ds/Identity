#include <stdafx.h>
#include <System.h>

using namespace Engine;

System::System() : m_window(800, 600, "Engine Window")
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

        if (m_window.m_keyboard.IsKeyDown('D'))
            OutputDebugString("D was pressed\n");
        if (m_window.m_keyboard.IsKeyUp('U'))
            OutputDebugString("U was pressed\n");
        if (m_window.m_keyboard.IsKeyHeld('H'))
            OutputDebugString("H was pressed\n");

    }

}

int System::Exit() const
{
    if (m_gResult == -1)
    {
        return -1;
    }

    return static_cast<int>(m_msg.wParam);
}
