#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Rendering/Camera.h>
#include <Containers/CameraContainer.h>

namespace Engine::Components
{
    class API_ENGINE CameraComponent: public IComponent
    {
    public:
        CameraComponent(Objects::GameObject* p_gameObject, int p_width, int p_height);
        ~CameraComponent() = default;

        bool operator==(IComponent* p_other) override;

        bool DeleteFromMemory() override;

        [[nodiscard]] inline std::shared_ptr<Rendering::Camera> GetCamera() const { return Containers::CameraContainer::GetCamera(m_camera); }

    private:
        int32_t m_camera{ -1 };
    };
}
