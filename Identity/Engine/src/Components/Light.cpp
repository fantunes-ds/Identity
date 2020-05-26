#include <stdafx.h>

#include <Components/Light.h>
#include <Systems/LightSystem.h>

Engine::Components::Light::Light(Objects::GameObject* p_gameObject): IComponent{p_gameObject, LIGHT}
{
    // Rendering::Lights::DirectionalLight* light = new Rendering::Lights::DirectionalLight();
    // Containers::LightContainer::AddLight(light);
    m_isActive = p_gameObject->IsActive();
    m_light = std::make_shared<Rendering::Lights::DirectionalLight>();
    Systems::LightSystem::AddLight(std::make_shared<Light>(*this));
}

Engine::Components::Light::Light(Objects::GameObject* p_gameObject, Rendering::Lights::DirectionalLight::LightData& p_lightData): IComponent{p_gameObject, LIGHT}
{
    m_isActive = p_gameObject->IsActive();
    m_light = std::make_shared<Rendering::Lights::DirectionalLight>(p_lightData);
    Systems::LightSystem::AddLight(std::make_shared<Light>(*this));
}


Engine::Components::Light::Light(Objects::GameObject* p_gameObject, std::shared_ptr<Light> p_other) : IComponent{p_gameObject, LIGHT}
{
    m_isActive = p_gameObject->IsActive();
    m_light = std::make_shared<Rendering::Lights::DirectionalLight>(p_other->GetLight()->GetLightData());
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

void Engine::Components::Light::SetActive(bool p_active)
{
    m_isActive = p_active;
}

void Engine::Components::Light::Serialize(std::ostream& p_stream)
{
    m_light->Serialize(p_stream);
}

void Engine::Components::Light::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
{
    m_gameObject = p_gameObject;

    auto light = std::make_shared<Rendering::Lights::DirectionalLight>();

    light->Deserialize(p_block);

    m_light->GetLightData() = light->GetLightData();

    Systems::LightSystem::AddLight(std::make_shared<Light>(*this));
}
