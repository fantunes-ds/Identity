#pragma once
#include <Export.h>
#include <map>
#include <Objects/GameObject.h>

namespace Engine::Containers
{
    class API_ENGINE GameObjectContainer
    {
    public:
        ~GameObjectContainer();

        GameObjectContainer(const GameObjectContainer&) = delete;
        GameObjectContainer(const GameObjectContainer&&) = delete;

        static int32_t AddGameObject(Objects::GameObject* p_gameObject);

        static GameObjectContainer* GetInstance();
        static std::map<int, std::shared_ptr<Objects::GameObject>>& GetAllGameObjects() { return GetInstance()->m_gameObjects; }

    private:
        GameObjectContainer() = default;

        inline static GameObjectContainer* m_instance = nullptr;
        std::map<int, std::shared_ptr<Objects::GameObject>> m_gameObjects;
    };
}
