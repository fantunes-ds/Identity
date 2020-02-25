#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Rendering/Lights/ILight.h>
#include <Containers/LightContainer.h>
#include <Rendering/Lights/Light.h>

namespace Engine::Components
{
    class API_ENGINE LightComponent: public IComponent
    {
    public:
        LightComponent();
        LightComponent(Rendering::Lights::Light::LightData p_lightData);
        bool operator==(IComponent* p_other) override;
        bool DeleteFromMemory() override;

        [[nodiscard]] std::shared_ptr<Rendering::Lights::ILight> GetLight() const { return nullptr; }

    private:
        int32_t m_light;
    };
}
