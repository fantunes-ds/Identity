#pragma once
#include <Export.h>
#include <map>
#include <memory>
#include <Components/IComponent.h>
#include <Containers/IContainer.h>

namespace Engine::Containers
{
    class API_ENGINE ComponentContainer
    {
    public:
        ~ComponentContainer();

        ComponentContainer(const ComponentContainer&) = delete;
        ComponentContainer(const ComponentContainer&&) = delete;

        template <class T, typename ...Args>
        static int32_t AddComponent(Objects::GameObject* p_gameObject, Args& ... p_args)
        {
            int32_t id = -1;

            //TODO: Possible memory leak here
            T* newComp = new T(p_gameObject, p_args...);
            // T* newCompEditor = new T(p_gameObject, p_args...);

            if (dynamic_cast<Components::IComponent*>(newComp)->GetID() >= 0)
                id = ComponentContainer::AddComponent(newComp);
            // if (dynamic_cast<Components::IComponent*>(newComp)->GetID() >= 0)
                // ComponentContainer::AddComponentEditor(newCompEditor);

            return id;
        }

        static void RemoveComponent(int32_t p_id, bool p_deleteFromMemory = true);
        static ComponentContainer* GetInstance();
        static std::map<int32_t, std::shared_ptr<Components::IComponent>>& GetAllComponents() { return GetInstance()->m_components; }
        static std::shared_ptr<Components::IComponent> FindComponent(int32_t p_id);

        template<class T>
        static std::vector<std::shared_ptr<T>> FindAllComponentsOfType()
        {
            std::vector<std::shared_ptr<T>> foundComps;

            for (const auto component : GetInstance()->m_components)
            {
                if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(FindComponent(component.first)))
                    foundComps.push_back(foundComp);
            }

            return foundComps;
        }

        static void CopyComp() { GetInstance()->CopyCompNS(); }
        static void SwitchComp() { GetInstance()->SwitchCompNS(); }
        void CopyCompNS();
        void SwitchCompNS();

        static int32_t AddComponent(Components::IComponent* p_component);
        static int32_t AddComponentEditor(Components::IComponent* p_component);
    private:
        ComponentContainer() = default;

        inline static ComponentContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Components::IComponent>> m_components;
        std::map<int32_t, std::shared_ptr<Components::IComponent>> m_componentsEditor;
    };
}
