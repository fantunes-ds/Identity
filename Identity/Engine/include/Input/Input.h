#pragma once

#include <Input/Keyboard.h>
#include <Input/Mouse.h>

// Macro for easier use everywhere in the project. _ is here because it would be a windows function otherwise.
#define _INPUT Input::Input::GetInstance()

namespace Engine::Input
{
    class API_ENGINE Input
    {
    public:
        Input() = default;
        ~Input() = default;

        Input(const Input&) = delete;
        Input(const Input&&) = delete;
        
        Input& operator=(const Input&) = delete;
        Input& operator=(const Input&&) = delete;

        /*
         * @brief initializes the singleton instance.
         */
        static void InitInput();

        /**
         * @brief Returns the Singleton instance.
         * @return The singleton instance reference.
         */
        static const std::unique_ptr<Input>& GetInstance() noexcept { return instance; }

        /*
         * @brief The Keyboard part of inputs.
         */
        Keyboard keyboard;
        /*
         * @brief The Mouse part of the inputs.
         */
        Mouse mouse;
    private:
        /**
         * @brief The static singleton instance
         */
        static std::unique_ptr<Input> instance;
    };
}
