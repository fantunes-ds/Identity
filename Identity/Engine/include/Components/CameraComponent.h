#pragma once
#include <Export.h>
#include <Components/IComponent.h>

namespace Engine::Components
{
    class API_ENGINE CameraComponent: public IComponent
    {
    public:
        CameraComponent() = default;

        void foo() override {}

    private:

    };
}
