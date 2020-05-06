#include <stdafx.h>

#include <Components/Light.h>
#include <Rendering/Lights/DirectionalLight.h>

Engine::Components::Light::Light(Objects::GameObject* p_gameObject): IComponent{p_gameObject}
{
    Rendering::Lights::DirectionalLight* light = new Rendering::Lights::DirectionalLight();
    m_light = light->GetID();
    Containers::LightContainer::AddLight(light);
}

Engine::Components::Light::Light(Objects::GameObject* p_gameObject, Rendering::Lights::DirectionalLight::LightData& p_lightData): IComponent{p_gameObject}
{
    Rendering::Lights::DirectionalLight* light = new Rendering::Lights::DirectionalLight(p_lightData);
    m_light = light->GetID();
    Containers::LightContainer::AddLight(light);
}

bool Engine::Components::Light::operator==(Engine::Components::IComponent* p_other)
{
    return false;
}

bool Engine::Components::Light::DeleteFromMemory()
{
    return false;
}

void Engine::Components::Light::Serialize(std::ostream& p_stream)
{
    Containers::LightContainer::FindLight(m_light)->Serialize(p_stream);
}

void Engine::Components::Light::Deserialize(std::vector<std::string>& p_block)
{
    Rendering::Lights::DirectionalLight light;

    light.Deserialize(p_block);

    Containers::LightContainer::AddLight(&light);
}
