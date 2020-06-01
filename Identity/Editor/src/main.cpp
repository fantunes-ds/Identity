#include <Tools/IdentityException.h>
#include <WinSetup.h>
#include <Core/Core.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    try
    {
        return Editor::Core::Core{}.Update();
    }
    catch (const Engine::Tools::IdentityException& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        MessageBox(nullptr, "UNKNOWN ERROR", "UNKNOWN EXCEPTION", MB_OK | MB_ICONERROR);
    }
}
