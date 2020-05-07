#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Objects/GameObject.h>
#include <Rendering/Lights/ILight.h>

namespace Engine::Components
{
    class API_ENGINE Light: public IComponent
    {
    public:
        Light() = delete;
        Light(Objects::GameObject* p_gameObject);
        Light(Objects::GameObject* p_gameObject, Rendering::Lights::ILight::LightData& p_lightData);

        [[nodiscard]] std::shared_ptr<Rendering::Lights::ILight> GetLight() const { return m_light; }

        bool operator==(IComponent* p_other) override;
        bool DeleteFromMemory() override;
        void SetActive(bool p_active) override { m_isActive = p_active; }

    private:
        std::shared_ptr<Rendering::Lights::ILight> m_light;
    };
}
