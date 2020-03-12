#pragma once
#include <Export.h>
#include <Components/Transform.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Containers/ModelContainer.h>
#include <Containers/ComponentContainer.h>
#include <Containers/ModelContainer.h>
#include <Components/IComponent.h>
#include <Components/ModelComponent.h>
#include <Scene/SceneGraph/SceneNode.h>

namespace Engine::Objects
{
    class API_ENGINE GameObject : public IObject
    {
    public:
        GameObject();
        GameObject(const std::string& p_name);
        ~GameObject() = default;

        //TODO: works when modifying parent's transform, but doesn't work when modifying child's transform
        void SetParentObject(GameObject& p_parent);

        bool RemoveComponent(int32_t p_id);

        template <class T, typename ...Args>
        int32_t AddComponent(Args& ... p_args)
        {
            if (std::is_same_v<T, Components::ModelComponent> && FindComponentOfType<Components::ModelComponent>())
            {
                std::string error("Tried to add a ModelComponent on object " + m_name + " while it already has one. Second ModelComponent has not been added.");
                MessageBox(nullptr, error.c_str(), "Warning", MB_OK | MB_ICONWARNING);
                return -1;
            }


            int32_t id = Containers::ComponentContainer::AddComponent<T>(this, p_args...);

            if (std::is_same_v<T, Components::ModelComponent>)
            {
                std::shared_ptr<Components::ModelComponent> modelComp = std::dynamic_pointer_cast<Components::ModelComponent>(Containers::ComponentContainer::FindComponent(id));
                Containers::ModelContainer::FindModel(modelComp->GetModel())->GetRootNode()->SetTransform(m_transform);
            }

            if (id > 0)
            {
                m_components.emplace_back(id);
            }

            return id;
        }

        /**
         * @return The first instance of the desired component type
         */
        std::shared_ptr<Components::IComponent> FindComponent(int32_t p_id) const
        {
            return Containers::ComponentContainer::FindComponent(p_id);
        }

        template <class T>
        std::shared_ptr<T> FindComponentOfType() const
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

        [[nodiscard]] std::shared_ptr<Components::Transform> GetTransform() const;
        [[nodiscard]] inline uint32_t GetTransformID() const { return m_transform; }
        [[nodiscard]] std::shared_ptr<ObjectElements::Model> GetModel() const;

        inline std::vector<int32_t>& GetAllComponents() { return m_components; }
        inline void SetTransform(int32_t p_transform) { m_transform = p_transform; }

        bool operator==(GameObject& p_other) const;

    private:
        int32_t m_transform = -1;
        std::vector<int32_t> m_components;
    };
}
