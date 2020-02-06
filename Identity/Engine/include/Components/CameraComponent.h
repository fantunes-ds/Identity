#pragma once
#include <Export.h>
#include <Components/IComponent.h>

namespace Engine::Components
{
    class API_ENGINE CameraComponent: public IComponent
    {
    public:
        CameraComponent(int p_width, int p_height);
        virtual ~CameraComponent() = default;

        bool IsWellInitialized() override;
        bool operator==(IComponent* p_other) override;

    private:
        int32_t m_camera = -1;
    };
}
