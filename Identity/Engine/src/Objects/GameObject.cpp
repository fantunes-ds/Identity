#include <stdafx.h>
#include <Objects/GameObject.h>
#include <Containers/ModelContainer.h>
#include <Containers/TransformContainer.h>
#include "Components/ModelComponent.h"
#include "Containers/GameObjectContainer.h"

Engine::Objects::GameObject::GameObject()
{
    ObjectElements::Transform transform{};
    m_transform = Containers::TransformContainer::AddTransform(transform);
    Containers::GameObjectContainer::AddGameObject(this);
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Objects::GameObject::GetTransform() const
{
    return Containers::TransformContainer::GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Objects::GameObject::GetModel() const
{
    for (auto& component: m_components)
    {
        if (Components::ModelComponent * modelComp = dynamic_cast<Components::ModelComponent*>(&*Containers::ComponentContainer::FindComponent(component)))
            return Containers::ModelContainer::FindModel(modelComp->GetModel());
    }

    return nullptr;
}

bool Engine::Objects::GameObject::operator==(GameObject& p_other) const
{
    if (m_transform == p_other.m_transform && m_components == p_other.m_components)
        return true;

    return false;
}

bool Engine::Objects::GameObject::RemoveComponent(int32_t p_id)
{
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        if (m_components[i] == p_id)
        {
            m_components.erase(m_components.begin() + i);
            Containers::ComponentContainer::RemoveComponent(p_id);
            return true;
        }
    }

    return false;
}
