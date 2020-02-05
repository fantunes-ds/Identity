#pragma once
#include <Export.h>
#include <map>
#include <Components/IComponent.h>

namespace Engine::Containers
{
    class API_ENGINE ComponentContainer
    {
    public:
        ~ComponentContainer();

        ComponentContainer(const ComponentContainer&) = delete;
        ComponentContainer(const ComponentContainer&&) = delete;

        //static int32_t AddGameObject(Objects::GameObject& p_gameObject);
        static int32_t AddComponent(Components::IComponent* p_component);

        static ComponentContainer* GetInstance();
        static std::map<int, std::shared_ptr<Components::IComponent>>& GetAllGameObjects() { return GetInstance()->m_gameObjects; }

    private:
        ComponentContainer() = default;

        inline static ComponentContainer* m_instance = nullptr;
        std::map<int, std::shared_ptr<Components::IComponent>> m_gameObjects;
    };
}
