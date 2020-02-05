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


        template <class T, typename ...Args>
        static int32_t AddComponent(Args& ... p_args)
        {
            int32_t id = -1;

            //TODO: Possible memory leak here
            T* newComp = new T(p_args...);

            if (dynamic_cast<Components::IComponent*>(newComp)->IsWellInitialized())
                id = ComponentContainer::AddComponent(newComp);

            return id;
        }

        static ComponentContainer* GetInstance();
        static std::map<int, std::shared_ptr<Components::IComponent>>& GetAllComponents() { return GetInstance()->m_components; }

        static std::shared_ptr<Components::IComponent> FindComponent(int32_t p_id);

        template<class T>
        static std::vector<std::shared_ptr<T>> FindAllComponentsOfType()
        {
            std::vector<std::shared_ptr<T>> foundComps;

            for (const auto component : GetInstance()->m_components)
            {
                if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<std::shared_ptr<T>>(FindComponent(component.first)))
                    foundComps.push_back(foundComp);
            }

            return foundComps;
        }

    private:
        ComponentContainer() = default;
        static int32_t AddComponent(Components::IComponent* p_component);

        inline static ComponentContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Components::IComponent>> m_components;
    };
}
