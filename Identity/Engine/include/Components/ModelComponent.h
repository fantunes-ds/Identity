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

        bool IsWellInitialized() override;

        bool operator==(IComponent* p_other) override;

        int32_t m_model;
    };
}
