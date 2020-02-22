#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include "3DLoader/ObjectElements/Model.h"
#include <Containers/ModelContainer.h>

namespace Engine::Components
{
    class API_ENGINE ModelComponent: public IComponent
    {
    public:
        ModelComponent(const std::string& p_name);
        ModelComponent(const std::string& p_file, const std::string& p_name);
        virtual ~ModelComponent() = default;

        bool operator==(IComponent* p_other) override;

        inline void SetModel(int32_t p_id) { m_model = p_id; }
        inline int32_t GetModel() const { return m_model; }

        bool DeleteFromMemory() override;

    private:
        int32_t m_model;
    };
}
