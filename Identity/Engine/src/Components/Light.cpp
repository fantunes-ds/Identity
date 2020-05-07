#include <stdafx.h>

#include <Components/Light.h>
#include <Rendering/Lights/DirectionalLight.h>

Engine::Components::Light::Light(Objects::GameObject* p_gameObject): IComponent{p_gameObject}
{
    auto light = std::make_shared<Rendering::Lights::DirectionalLight>();
    m_light = light->GetID();
    Containers::LightContainer::AddLight(light);
}

Engine::Components::Light::Light(Objects::GameObject* p_gameObject, Rendering::Lights::DirectionalLight::LightData& p_lightData): IComponent{p_gameObject}
{
    //TODO: change to ILight once class is complete
    auto light = std::make_shared<Rendering::Lights::DirectionalLight>(p_lightData);
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

void Engine::Components::Light::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
{
    m_gameObject = p_gameObject;

    auto light = std::make_shared<Rendering::Lights::DirectionalLight>();

    light->Deserialize(p_block);

    Containers::LightContainer::AddLight(light);
}
