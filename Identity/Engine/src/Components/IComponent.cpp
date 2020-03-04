#include <stdafx.h>
#include <Components/IComponent.h>
#include <Objects/GameObject.h>

Engine::Components::IComponent::IComponent(Objects::GameObject* p_gameObject): m_gameObject {p_gameObject} {}

void Engine::Components::IComponent::SetGameObject(Objects::GameObject* p_gameObject)
{
    m_gameObject = p_gameObject;
}

std::shared_ptr<Engine::Objects::GameObject> Engine::Components::IComponent::GetGameObject() const
{
    return std::shared_ptr<Objects::GameObject>(m_gameObject);
}
