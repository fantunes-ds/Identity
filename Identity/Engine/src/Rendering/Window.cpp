#include <stdafx.h>
#include <Rendering/Window.h>
#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_win32.h>
#include <Input/Input.h>

using namespace Engine::Rendering;

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

Window::Window(int p_width, int p_height, const char* p_name) : m_width(p_width), m_height(p_height)
{
    //adjust the size ot he window so the coosen resolution is for the user view and not for the entire window
    RECT wr;
    wr.left = 100;
    wr.right = p_width + wr.left;
    wr.top = 100;
    wr.bottom = p_height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    m_hwnd = CreateWindow(WindowClass::GetName(), p_name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
        wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::GetInstance(), this);

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    ImGui_ImplWin32_Init(m_hwnd);

    m_renderer = std::make_unique<Renderer>(m_hwnd, m_width, m_height);
    isSet = true;
}

Window::~Window()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(m_hwnd);
}

Renderer& Window::GetRenderer() const
{
    return *m_renderer;
}

void Window::SetTitle(const std::string& title) const
{
    SetWindowText(m_hwnd, title.c_str());
}

std::optional<int> Window::ProcessMessage()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

LRESULT Window::HandleMsgSetup(const HWND p_hwnd, const UINT p_msg, const WPARAM p_wParam, const LPARAM p_lParam)
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

LRESULT Window::HandleMsgThunk(const HWND p_hwnd, const UINT p_msg, const WPARAM p_wParam, const LPARAM p_lParam)
{
    Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(p_hwnd, GWLP_USERDATA));
    return window->HandleMsg(p_hwnd, p_msg, p_wParam, p_lParam);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND p_hwnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);
LRESULT Window::HandleMsg(const HWND p_hwnd, const UINT p_msg, const WPARAM p_wParam, const LPARAM p_lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(p_hwnd, p_msg, p_wParam, p_lParam))
        return true;
    // no default switch case because windows sends a lot of different
    // random unknown messages, and we don't need to filter them all.
    switch(p_msg)
    {
    case WM_SIZING:
        if (isSet)
        {
            RECT rcClient;
            GetClientRect(m_hwnd, &rcClient);
            m_width = rcClient.right - rcClient.left;
            m_height = rcClient.bottom - rcClient.top;
            m_renderer->Resize(m_width, m_height);
        }
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

        // clear keystate when window loses focus to prevent input getting "stuck"
    case WM_KILLFOCUS:
        Input::Input::GetInstance().get()->keyboard.ClearStates();
        break;

        /*********** KEYBOARD MESSAGES ***********/
    case WM_KEYDOWN:
        // sys-key commands need to be handled to track ALT key (VK_MENU) and F10
    case WM_SYSKEYDOWN:
        if (!(p_lParam & 0x40000000) || _INPUT->keyboard.IsAutoRepeatEnabled()) // filter auto-repeat
        {
            _INPUT->keyboard.OnKeyPressed(static_cast<unsigned char>(p_wParam));
        }
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        _INPUT->keyboard.OnKeyReleased(static_cast<unsigned char>(p_wParam));
        break;
    case WM_CHAR:
        _INPUT->keyboard.OnChar(static_cast<unsigned char>(p_wParam));
        break;
        /*********** END KEYBOARD MESSAGES ***********/

    /************* MOUSE MESSAGES ****************/
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(p_lParam);
        if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
        {
            _INPUT->mouse.OnMouseMove(pt.x, pt.y);
            if (!_INPUT->mouse.IsInWindow())
            {
                SetCapture(p_hwnd);
                _INPUT->mouse.OnMouseEnter();
            }
        }
        else
        {
            if (p_wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                _INPUT->mouse.OnMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                _INPUT->mouse.OnMouseLeave();
            }
        }
        break;
    }
    case WM_LBUTTONDOWN:
    {
        _INPUT->mouse.OnLeftPressed();
        break;
    }
    case WM_RBUTTONDOWN:
    {
        _INPUT->mouse.OnRightPressed();
        break;
    }
    case WM_LBUTTONUP:
    {
        _INPUT->mouse.OnLeftReleased();
        break;
    }
    case WM_RBUTTONUP:
    {
        _INPUT->mouse.OnRightReleased();
        break;
    }
    case WM_MOUSEWHEEL:
    {
        if (GET_WHEEL_DELTA_WPARAM(p_wParam) > 0)
        {
            _INPUT->mouse.OnWheelUp();
        }
        else if (GET_WHEEL_DELTA_WPARAM(p_wParam) < 0)
        {
            _INPUT->mouse.OnWheelDown();
        }
        break;
    }
    /************** END MOUSE MESSAGES **************/
    }

    return DefWindowProc(p_hwnd, p_msg, p_wParam, p_lParam);
}
