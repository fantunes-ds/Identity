#include <stdafx.h>

#include <Containers/ComponentContainer.h>
#include <Containers/GameObjectContainer.h>
#include <Components/ModelComponent.h>
#include <Managers/ResourceManager.h>
#include <Managers/SceneManager.h>
#include <Objects/GameObject.h>
#include <Systems/TransformSystem.h>
#include <Scene/Scene.h>

using namespace Engine::Objects;

GameObject::GameObject()
{
    m_transform = Systems::TransformSystem::AddTransform(std::make_shared<Components::Transform>());
    Containers::GameObjectContainer::AddGameObject(std::shared_ptr<GameObject>(this));
    auto trm = Systems::TransformSystem::GetTransform(m_transform);
    Containers::ComponentContainer::AddComponent(trm.get());
}

GameObject::GameObject(const std::string& p_name)
{
    m_transform = Systems::TransformSystem::AddTransform(std::make_shared<Components::Transform>(p_name));
    Containers::GameObjectContainer::AddGameObject(std::shared_ptr<GameObject>(this));
    SetName(p_name);
    auto trm = Systems::TransformSystem::GetTransform(m_transform);
    Containers::ComponentContainer::AddComponent(trm.get());
}

void GameObject::DeleteFromMemory()
{
    for (auto& component : m_components)
    {
        auto p = Containers::ComponentContainer::FindComponent(component);
        Containers::ComponentContainer::RemoveComponent(component);
    }

    Containers::GameObjectContainer::RemoveGameObject(GetID());

    Managers::SceneManager::GetActiveScene()->RemoveGameObject(GetID());
}

std::shared_ptr<Engine::Components::Transform> GameObject::GetTransform() const
{
    return Systems::TransformSystem::GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> GameObject::GetModel() const
{
    if (this == nullptr)
        return nullptr;

    for (auto& component : m_components)
    {
        if (Components::ModelComponent* modelComp = dynamic_cast<Components::ModelComponent*>(&*Containers::ComponentContainer::FindComponent(component)))
            return Managers::ResourceManager::FindModel(modelComp->GetModel());
    }

    return nullptr;
}

inline void GameObject::SetActive(bool p_active)
{
    for (auto& component : GetAllComponents())
    {
        Containers::ComponentContainer::FindComponent(component)->SetActive(p_active);
    }

    m_isActive = p_active;
}

bool GameObject::operator==(GameObject& p_other) const
{
    if (m_transform == p_other.m_transform && m_components == p_other.m_components)
        return true;

    return false;
}

void GameObject::SetParentObject(std::shared_ptr<GameObject> p_parent)
{
    p_parent->GetSceneNode()->AddChild(m_rootNode);
    //Managers::SceneManager::GetActiveScene()->GetSceneGraph().UpdateScene(0.0f);
}

bool GameObject::RemoveComponent(int32_t p_id)
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
