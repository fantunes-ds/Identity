#include <stdafx.h>
#include <Input/Input.h>

using namespace Engine::Input;
std::unique_ptr<Input> Input::instance;

void Input::InitInput()
{
    if (instance == nullptr)
        instance = std::make_unique<Input>();
}
