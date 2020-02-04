#pragma once
#include <Export.h>
#include <map>
#include <Objects/GameObject.h>

namespace Engine::Managers
{
    //Singleton or no?
    class API_ENGINE GameObjectManager
    {
    public:
        ~GameObjectManager();

        GameObjectManager(const GameObjectManager&) = delete;
        GameObjectManager(const GameObjectManager&&) = delete;

        static int32_t AddGameObject(Objects::GameObject& p_gameObject);

        static GameObjectManager* GetInstance();
        static std::map<int, std::shared_ptr<Objects::GameObject>>& GetAllGameObjects() { return GetInstance()->m_gameObjects; }
    private:
        GameObjectManager() = default;

        inline static GameObjectManager* m_instance = nullptr;
        std::map<int, std::shared_ptr<Objects::GameObject>> m_gameObjects;
    };
}
