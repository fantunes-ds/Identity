#include <stdafx.h>
#include <Objects/GameObject.h>
#include <Systems/TransformSystem.h>
#include <Components/ModelComponent.h>
#include <Containers/GameObjectContainer.h>
#include <Scene/Scene.h>
#include <Managers/SceneManager.h>
#include <Managers/ResourceManager.h>

Engine::Objects::GameObject::GameObject()
{
    m_transform = Containers::TransformSystem::AddTransform(std::make_shared<Components::Transform>());
    Containers::GameObjectContainer::AddGameObject(std::shared_ptr<GameObject>(this));
}

Engine::Objects::GameObject::GameObject(const std::string& p_name)
{
    m_transform = Containers::TransformSystem::AddTransform(std::make_shared<Components::Transform>(p_name));
    Containers::GameObjectContainer::AddGameObject(std::shared_ptr<GameObject>(this));
    SetName(p_name);
}

std::shared_ptr<Engine::Components::Transform> Engine::Objects::GameObject::GetTransform() const
{
    return Containers::TransformSystem::GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Objects::GameObject::GetModel() const
{
    for (auto& component : m_components)
    {
        if (Components::ModelComponent* modelComp = dynamic_cast<Components::ModelComponent*>(&*Containers::ComponentContainer::FindComponent(component)))
            return Managers::ResourceManager::FindModel(modelComp->GetModel());
    }

    return nullptr;
}

bool Engine::Objects::GameObject::operator==(GameObject& p_other) const
{
    if (m_transform == p_other.m_transform && m_components == p_other.m_components)
        return true;

    return false;
}

void Engine::Objects::GameObject::SetParentObject(std::shared_ptr<GameObject> p_parent)
{
    p_parent->GetSceneNode()->AddChild(m_rootNode);
    //Managers::SceneManager::GetActiveScene()->GetSceneGraph().UpdateScene(0.0f);
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
