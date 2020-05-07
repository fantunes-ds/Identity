#include <stdafx.h>

#include <Components/Light.h>
#include <Rendering/Lights/DirectionalLight.h>

using namespace Engine::Components;

Light::Light(Objects::GameObject* p_gameObject): IComponent{p_gameObject, LIGHT}
{
    Rendering::Lights::DirectionalLight* light = new Rendering::Lights::DirectionalLight();
    Containers::LightContainer::AddLight(light);
}

Light::Light(Objects::GameObject* p_gameObject, Rendering::Lights::DirectionalLight::LightData& p_lightData): IComponent{p_gameObject, LIGHT}
{
    Rendering::Lights::DirectionalLight* light = new Rendering::Lights::DirectionalLight(p_lightData);
    Containers::LightContainer::AddLight(light);
}

bool Light::operator==(IComponent* p_other)
{
    return false;
}

bool Light::DeleteFromMemory()
{
    return false;
}
