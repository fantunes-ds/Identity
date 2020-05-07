#include <stdafx.h>

#include <Components/Light.h>
#include <Systems/LightSystem.h>

Engine::Components::Light::Light(Objects::GameObject* p_gameObject): IComponent{p_gameObject}
{
    // Rendering::Lights::DirectionalLight* light = new Rendering::Lights::DirectionalLight();
    // Containers::LightContainer::AddLight(light);
    m_light = std::make_shared<Rendering::Lights::DirectionalLight>();
    Systems::LightSystem::AddLight(std::make_shared<Light>(*this));
}

Engine::Components::Light::Light(Objects::GameObject* p_gameObject, Rendering::Lights::DirectionalLight::LightData& p_lightData): IComponent{p_gameObject}
{
    m_light = std::make_shared<Rendering::Lights::DirectionalLight>(p_lightData);
    Systems::LightSystem::AddLight(std::make_shared<Light>(*this));
}

bool Engine::Components::Light::operator==(Engine::Components::IComponent* p_other)
{
    return false;
}

bool Engine::Components::Light::DeleteFromMemory()
{
    return false;
}
