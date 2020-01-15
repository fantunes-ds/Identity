// #include <Window.h>
#include <System.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    Engine::System sys(800, 600, "Test Window");
    return sys.Exit();
}