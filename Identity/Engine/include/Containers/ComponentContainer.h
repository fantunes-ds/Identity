#pragma once
#include <Export.h>
#include <map>
#include <memory>
#include <Components/IComponent.h>

namespace Engine::Containers
{
    class API_ENGINE ComponentContainer
    {
    public:
        ~ComponentContainer();

        ComponentContainer(const ComponentContainer&) = delete;
        ComponentContainer(const ComponentContainer&&) = delete;

        static int32_t AddComponent(Components::IComponent* p_component);

        template <class T, typename ...Args>
        static int32_t AddComponent(Args& ... p_args)
        {
            int32_t id = -1;
            T* newComp = new T(p_args...);

            if (dynamic_cast<Components::IComponent*>(newComp)->IsWellInitialized())
                id = Containers::ComponentContainer::AddComponent(newComp);

            return id;
        }

        static ComponentContainer* GetInstance();
        static std::map<int, std::shared_ptr<Components::IComponent>>& GetAllComponents() { return GetInstance()->m_components; }

        static std::shared_ptr<Components::IComponent> FindComponent(int32_t p_id);

    private:
        ComponentContainer() = default;

        inline static ComponentContainer* m_instance = nullptr;
        std::map<int, std::shared_ptr<Components::IComponent>> m_components;
    };
}
