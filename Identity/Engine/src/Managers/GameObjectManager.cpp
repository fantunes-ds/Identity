#include <stdafx.h>
#include <Managers/GameObjectManager.h>

Engine::Managers::GameObjectManager::~GameObjectManager()
{
    delete m_instance;
}

int32_t Engine::Managers::GameObjectManager::AddGameObject(Objects::GameObject& p_gameObject)
{
    for (auto& gameObject: GetInstance()->m_gameObjects)
    {
        if (p_gameObject == *gameObject.second)
        {
            const std::string error("GameObjectManager::AddGameObject(Objects::GameObject& p_gameObject): Failed to add GameObject because it already exists");
            MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
            return -1;
        }
    }

    int32_t id = Tools::IDCounter::GetNewID();
    GetInstance()->m_gameObjects.insert_or_assign(id, std::make_shared<Objects::GameObject>(p_gameObject));
    return id;
}

Engine::Managers::GameObjectManager* Engine::Managers::GameObjectManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new GameObjectManager();
    }

    return m_instance;
}
