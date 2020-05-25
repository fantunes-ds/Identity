#include <stdafx.h>

#include <Containers/GameObjectContainer.h>

#include "Systems/TransformSystem.h"

Engine::Containers::GameObjectContainer::~GameObjectContainer()
{
    delete m_instance;
}

int32_t Engine::Containers::GameObjectContainer::AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    if (p_gameObject == nullptr)
        return -1;

    for (auto& gameObject : GetInstance()->m_gameObjects)
    {
        if (*p_gameObject == *gameObject.second)
        {
            const std::string error("GameObjectContainer::AddGameObjectToScene(Objects::GameObject& p_gameObject): Failed to add GameObject because it already exists");
            MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
            return -1;
        }
    }

    GetInstance()->m_gameObjects.insert_or_assign(p_gameObject->GetID(), std::shared_ptr<Engine::Objects::GameObject>(p_gameObject));
    return p_gameObject->GetID();
}

void Engine::Containers::GameObjectContainer::RemoveGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    if (!p_gameObject)
        return;

    for (auto& component : p_gameObject->GetAllComponents())
    {
        ComponentContainer::RemoveComponent(component);
    }

    //GetInstance()->m_gameObjects.erase(p_gameObject->GetID());
}

void Engine::Containers::GameObjectContainer::RemoveGameObject(int32_t p_id)
{
    if (p_id < 0)
        return;

    auto go = GetInstance()->m_gameObjects.find(p_id);

    GetInstance()->m_gameObjects.erase(p_id);
}

Engine::Containers::GameObjectContainer* Engine::Containers::GameObjectContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new GameObjectContainer();
    }

    return m_instance;
}
