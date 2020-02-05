#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Containers/ModelContainer.h>
#include <Containers/ComponentContainer.h>
#include <Components/IComponent.h>
#include <Components/ModelComponent.h>

namespace Engine::Objects
{
    class API_ENGINE GameObject
    {
    public:
        GameObject();
        ~GameObject() = default;

        [[nodiscard]] std::shared_ptr<ObjectElements::Transform> GetTransform() const;
        [[nodiscard]] inline uint32_t GetTransformID() const { return m_transform; }
        [[nodiscard]] std::shared_ptr<ObjectElements::Model> GetModel() const;

        inline std::vector<int32_t>& GetAllComponents() { return m_components; }
        inline void SetTransform(int32_t p_transform) { m_transform = p_transform; }

        bool operator==(GameObject& p_other) const;

        template <class T, typename ...Args>
        void AddComponent(Args& ... p_args)
        {
            int32_t id = Containers::ComponentContainer::AddComponent<T>(p_args...);

            if (id > 0)
                m_components.emplace_back(id);
        }

        /**
         * @return The first instance of the desired component type
         */
        template <class T>
        std::shared_ptr<T> FindComponent(int32_t p_id = -1) const
        {
            for (auto component : m_components)
            {
                if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(Containers::ComponentContainer::FindComponent(component)))
                return foundComp;
            }

            return nullptr;
        }

        template <class T>
        std::vector<std::shared_ptr<T>> FindAllComponentsOfType() const
        {
            std::vector<std::shared_ptr<T>> foundComps;

            for (auto component : m_components)
            {
                if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(Containers::ComponentContainer::FindComponent(component)))
                    foundComps.push_back(foundComp);
            }

            return foundComps;
        }

    private:
        int32_t m_transform = -1;
        std::vector<int32_t> m_components;
    };
}
