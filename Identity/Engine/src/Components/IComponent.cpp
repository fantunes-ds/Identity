#include <stdafx.h>

#include <Components/IComponent.h>
#include <Objects/GameObject.h>

using namespace Engine::Components;

IComponent::IComponent(Objects::GameObject* p_gameObject, const Type& p_type) : m_gameObject{p_gameObject}, m_type(p_type) {}

void IComponent::SetGameObject(Objects::GameObject* p_gameObject)
{
    m_gameObject = p_gameObject;
}

Engine::Objects::GameObject* IComponent::GetGameObject() const
{
    return m_gameObject;
}
