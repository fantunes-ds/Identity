#include <stdafx.h>
#include <Window.h>

using namespace Engine::Renderer;

Window::WindowClass Window::WindowClass::m_windowClass;

const char* Window::WindowClass::GetName()
{
    return m_windowClassName;
}

HINSTANCE Window::WindowClass::GetInstance()
{
    return m_windowClass.m_hinstance;
}

Window::WindowClass::WindowClass() : m_hinstance(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(m_windowClassName, GetInstance());
}

Window::Window(int p_width, int p_height, const char* p_name)
{
    // adjust window so that the resolution is for the user view not the entire window
    RECT wr;
    wr.left = 100;
    wr.right = p_width + wr.left;
    wr.top = 100;
    wr.bottom = p_height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    //create the window and get the window handle
    m_hwnd = CreateWindow(WindowClass::GetName(), p_name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 
        wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
    DestroyWindow(m_hwnd);
}

LRESULT Window::HandleMsgSetup(HWND p_hwnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
    if (p_msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(p_lParam);
        Window* const window = static_cast<Window*>(create->lpCreateParams);
        SetWindowLongPtr(p_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        SetWindowLongPtr(p_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        return window->HandleMsg(p_hwnd, p_msg, p_wParam, p_lParam);
    }
    return DefWindowProc(p_hwnd, p_msg, p_wParam, p_lParam);
}

LRESULT Window::HandleMsgThunk(HWND p_hwnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
    Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(p_hwnd, GWLP_USERDATA));
    return window->HandleMsg(p_hwnd, p_msg, p_wParam, p_lParam);
}

LRESULT Window::HandleMsg(HWND p_hwnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
    // no default switch case because windows sends a lot of different
    // random unknown messages, and we don't need to filter them all.
    switch(p_msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

        // clear keystate when window loses focus to prevent input getting "stuck"
    case WM_KILLFOCUS:
        m_keyboard.ClearStates();
        break;

        /*********** KEYBOARD MESSAGES ***********/
    case WM_KEYDOWN:
        // sys-key commands need to be handled to track ALT key (VK_MENU) and F10
    case WM_SYSKEYDOWN:
        if (!(p_lParam & 0x40000000) || m_keyboard.IsAutoRepeatEnabled()) // filter auto-repeat
        {
            m_keyboard.OnKeyPressed(static_cast<unsigned char>(p_wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        m_keyboard.OnKeyReleased(static_cast<unsigned char>(p_wParam));
        break;
    case WM_CHAR:
        m_keyboard.OnChar(static_cast<unsigned char>(p_wParam));
        break;
        /*********** END KEYBOARD MESSAGES ***********/
    }

    return DefWindowProc(p_hwnd, p_msg, p_wParam, p_lParam);
}