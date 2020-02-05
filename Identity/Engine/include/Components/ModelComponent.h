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
        bool IsWellInitialized() override;

        ModelComponent(const std::string& p_name)
        {
            m_model = Containers::ModelContainer::FindModel(p_name);
        }

        ModelComponent(const std::string& p_file, const std::string& p_name)
        {
            Containers::ModelContainer::AddModel(p_file, p_name);
            m_model = Containers::ModelContainer::FindModel(p_name);
        }

        int32_t m_model;
    };
}
