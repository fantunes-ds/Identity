#pragma once

#include <Input/Keyboard.h>
#include <Input/Mouse.h>

namespace Engine::Input
{
    class API_ENGINE Input
    {
    public:
        Input() = delete;
        ~Input() = delete;

        Input(const Input&) = delete;
        Input(const Input&&) = delete;
        
        Input& operator=(const Input&) = delete;
        Input& operator=(const Input&&) = delete;

        static void InitInput();

        static const std::unique_ptr<Input>& GetInstance() noexcept { return instance; }
        Keyboard keyboard;
        Mouse mouse;
    private:
        static std::unique_ptr<Input> instance;
    };
}
