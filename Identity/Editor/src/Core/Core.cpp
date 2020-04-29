#include <Core/Core.h>
#include <Tools/IdentityException.h>

Editor::Core::Core::Core()
{
    
}

Editor::Core::Core::~Core()
{
}

void Editor::Core::Core::Init()
{
}

void Editor::Core::Core::Shutdown()
{
}

int Editor::Core::Core::Update()
{
    try
    {
        return m_engine.Run();
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


    return 0;
}

