#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Containers/ModelContainer.h>
#include <Containers/ComponentContainer.h>
#include <Components/IComponent.h>

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

        inline std::vector<uint32_t>& GetAllComponents() { return m_components; }
        inline void SetTransform(uint32_t p_transform) { m_transform = p_transform; }

        bool operator==(GameObject& p_other) const;

        template <class T, typename ...Args>
        void AddComponent(Args& ... p_args)
        {
            int32_t id = Containers::ComponentContainer::AddComponent<T>(p_args...);

            if (id > 0)
                m_components.emplace_back(id);
        }

    private:
        uint32_t m_transform = -1;
        std::vector<uint32_t> m_components;
    };
}
