#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <3DLoader/ObjectElements/Model.h>
#include "Managers/ModelManager.h"

namespace Engine::Objects
{
    class API_ENGINE GameObject
    {
    public:
        GameObject() = default;
        ~GameObject() = default;

        [[nodiscard]] std::shared_ptr<ObjectElements::Transform> GetTransform() const;
        inline [[nodiscard]] uint32_t GetTransformID() const { return m_transform; }
        [[nodiscard]] std::shared_ptr<ObjectElements::Model> GetModel() const;
        [[nodiscard]] uint32_t GetModelID() const { return m_model; };

        void SetModel(const std::string& p_name);
        inline void SetModel(uint32_t p_model) { m_model = p_model; }
        inline void SetTransform(uint32_t p_transform) { m_transform = p_transform; }

        bool operator==(GameObject& p_other) const;

    private:
        //TODO:make this into a vector of components
        uint32_t m_model;
        uint32_t m_transform;
    };
}
