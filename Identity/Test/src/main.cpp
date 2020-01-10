// #include <Window.h>
#include <System.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    Engine::System sys(1920, 1080, "Test Window");
    sys.Run();
    return sys.Exit();
}