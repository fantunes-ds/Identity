#include <Core/App.h>
#include <Tools/IdentityException.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    try
    {
        Engine::Core::App engine = { 1280, 720, "Identity (Beta)" };
        engine.Init();
        return engine.Run();
    }
    catch(const Engine::Tools::IdentityException& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONERROR);
    }
    catch(const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONERROR);
    }
    catch(...)
    {
        MessageBox(nullptr, "UNKNOWN ERROR", "UNKNOWN EXCEPTION", MB_OK | MB_ICONERROR);
    }
}
