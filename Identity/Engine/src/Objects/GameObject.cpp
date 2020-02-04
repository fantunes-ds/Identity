#include <stdafx.h>
#include <Objects/GameObject.h>
#include <Managers/ModelManager.h>
#include <Managers/TransformManager.h>
#include "Components/ModelComponent.h"
#include "Managers/GameObjectManager.h"

Engine::Objects::GameObject::GameObject()
{
    ObjectElements::Transform transform{};
    m_transform = Managers::TransformManager::AddTransform(transform);
    Managers::GameObjectManager::AddGameObject(this);
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Objects::GameObject::GetTransform() const
{
    return Managers::TransformManager::GetInstance()->GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Objects::GameObject::GetModel() const
{
    for (auto& component: m_components)
    {
        if (Components::ModelComponent * modelComp = dynamic_cast<Components::ModelComponent*>(component))
            return Managers::ModelManager::GetInstance()->FindModel(modelComp->m_model);
    }

    return nullptr;
}

/*void Engine::Objects::GameObject::SetModel(const std::string& p_name)
{
    int32_t id = Managers::ModelManager::FindModel(p_name);

    if (id < 0)
    {
        const std::string error("GameObject::SetModel(const std::string& p_name): Could not find model with name " + p_name + "\n");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return;
    }

    m_model = id;
}*/

bool Engine::Objects::GameObject::operator==(GameObject& p_other) const
{
    //TODO: make this work once the rest works
    if (m_transform == p_other.m_transform && false)
        return true;

    return false;
}
