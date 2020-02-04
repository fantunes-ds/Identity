#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <3DLoader/ObjectElements/Model.h>
#include "Managers/ModelManager.h"
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

        inline std::vector<Components::IComponent*>& GetAllComponents() { return m_components; }
        //[[nodiscard]] uint32_t GetModelID() const { return m_model; };

        /*void SetModel(const std::string& p_name);
        inline void SetModel(uint32_t p_model) { m_model = p_model; }*/
        inline void SetTransform(uint32_t p_transform) { m_transform = p_transform; }

        bool operator==(GameObject& p_other) const;

        template <class T, typename ...Args>
        void AddComponent(Args&... p_args)
        {
            T* newComp = new T( p_args... );

            m_components.emplace_back(newComp);
        }

    private:
        //TODO:make this into a vector of components
        uint32_t m_transform = -1;
        std::vector<Components::IComponent*> m_components;
    };
}
