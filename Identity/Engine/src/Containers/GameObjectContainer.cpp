#include <stdafx.h>
#include <Containers/GameObjectContainer.h>
#include <Tools/IDCounter.h>

Engine::Containers::GameObjectContainer::~GameObjectContainer()
{
    delete m_instance;
}

int32_t Engine::Containers::GameObjectContainer::AddGameObject(Objects::GameObject* p_gameObject)
{
    for (auto& gameObject : GetInstance()->m_gameObjects)
    {
        if (*p_gameObject == *gameObject.second)
        {
            const std::string error("GameObjectContainer::AddGameObject(Objects::GameObject& p_gameObject): Failed to add GameObject because it already exists");
            MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
            return -1;
        }
    }

    int32_t id = Tools::IDCounter::GetNewID();
    GetInstance()->m_gameObjects.insert_or_assign(id, std::shared_ptr<Engine::Objects::GameObject>(p_gameObject));
    return id;
}

Engine::Containers::GameObjectContainer* Engine::Containers::GameObjectContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new GameObjectContainer();
    }

    return m_instance;
}
