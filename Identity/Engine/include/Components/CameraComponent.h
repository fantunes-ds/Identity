#pragma once
#include <Export.h>
#include <Components/IComponent.h>

namespace Engine::Components
{
    class API_ENGINE CameraComponent: public IComponent
    {
    public:
        CameraComponent() = default;
        virtual ~CameraComponent() = default;

        bool IsWellInitialized() override;
        bool operator==(IComponent* p_other) override;
    };
}
