#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include "3DLoader/ObjectElements/Model.h"

namespace Engine::Components
{
    class API_ENGINE ModelComponent: public IComponent
    {
    public:
        ModelComponent(Objects::GameObject* p_gameObject, std::shared_ptr<ModelComponent> p_other);
        ModelComponent(Objects::GameObject* p_gameObject, const int32_t p_id);
        ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_name);
        ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_file, const std::string& p_name);
        virtual ~ModelComponent() = default;

        void Serialize(std::ostream& p_stream) override;

        bool operator==(IComponent* p_other) override;

        inline void SetModel(int32_t p_id) { m_model = p_id; }
        inline int32_t GetModel() const { return m_model; }

        bool DeleteFromMemory() override;
        void SetActive(bool p_active) override { m_isActive = p_active; }

    private:
        int32_t m_model;
    };
}
