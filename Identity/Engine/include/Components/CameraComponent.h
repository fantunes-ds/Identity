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

        bool IsWellInitialized() override { return true; }
        bool operator==(IComponent* p_other) override { return false; }

    private:
        int32_t m_id = -1;
    };
}
