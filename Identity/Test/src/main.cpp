// #include <Window.h>
#include <System.h>
#include "TestDependencies.h"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
	Test test;
	test.TestAssimp();
	//test.TestIrrKlang();
	test.TestGPM();

    Engine::System sys;
    sys.Run();
    return sys.Exit();
}
