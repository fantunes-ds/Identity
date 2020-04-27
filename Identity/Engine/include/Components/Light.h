#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Rendering/Lights/ILight.h>
#include <Containers/LightContainer.h>
#include <Rendering/Lights/DirectionalLight.h>

namespace Engine::Components
{
    class API_ENGINE Light: public IComponent
    {
    public:
        Light(Objects::GameObject* p_gameObject);
        Light(Objects::GameObject* p_gameObject, Rendering::Lights::DirectionalLight::LightData& p_lightData);
        bool operator==(IComponent* p_other) override;
        bool DeleteFromMemory() override;

        [[nodiscard]] std::shared_ptr<Rendering::Lights::ILight> GetLight() const { return nullptr; }

    private:
        int32_t m_light;
    };
}
