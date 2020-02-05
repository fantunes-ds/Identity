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
    return Containers::TransformContainer::GetInstance()->GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Objects::GameObject::GetModel() const
{
    for (auto& component: m_components)
    {
        if (Components::ModelComponent * modelComp = dynamic_cast<Components::ModelComponent*>(component))
            return Containers::ModelContainer::GetInstance()->FindModel(modelComp->m_model);
    }

    return nullptr;
}

/*void Engine::Objects::GameObject::SetModel(const std::string& p_name)
{
    int32_t id = Containers::ModelContainer::FindModel(p_name);

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
