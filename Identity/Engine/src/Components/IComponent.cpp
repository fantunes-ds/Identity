#include <stdafx.h>
#include <Components/IComponent.h>
#include <memory>
#include <utility>
#include <Objects/GameObject.h>

Engine::Components::IComponent::IComponent(Objects::GameObject* p_gameObject): m_gameObject { p_gameObject } {}

void Engine::Components::IComponent::SetGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    m_gameObject = std::move(p_gameObject);
}

std::shared_ptr<Engine::Objects::GameObject> Engine::Components::IComponent::GetGameObject() const
{
    return m_gameObject;
}
