#include <stdafx.h>

#include <Containers/GameObjectContainer.h>

using namespace Engine::Containers;

GameObjectContainer::~GameObjectContainer()
{
    delete m_instance;
}

int32_t GameObjectContainer::AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
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

bool GameObjectContainer::RemoveGameObject(int32_t p_id)
{
    //size_t before = GetInstance()->m_gameObjects.size();
    //GetInstance()->m_gameObjects.erase(p_id);
    //size_t after = GetInstance()->m_gameObjects.size();

    return true;
}

GameObjectContainer* Engine::Containers::GameObjectContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new GameObjectContainer();
    }

    return m_instance;
}
