#include <stdafx.h>
#include <Components/LightComponent.h>
#include <Rendering/Lights/Light.h>

Engine::Components::LightComponent::LightComponent(Objects::GameObject* p_gameObject): IComponent{ p_gameObject }
{

    Rendering::Lights::Light* light = new Rendering::Lights::Light();
    Containers::LightContainer::AddLight(light);
}

Engine::Components::LightComponent::LightComponent(Objects::GameObject* p_gameObject, Rendering::Lights::Light::LightData& p_lightData): IComponent{ p_gameObject }
{
    Rendering::Lights::Light* light = new Rendering::Lights::Light(p_lightData);
    Containers::LightContainer::AddLight(light);
}

bool Engine::Components::LightComponent::operator==(IComponent* p_other)
{
    return false;
}

bool Engine::Components::LightComponent::DeleteFromMemory()
{
    return false;
}

