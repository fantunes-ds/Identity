#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include "3DLoader/ObjectElements/Model.h"
#include "Managers/ModelManager.h"

namespace Engine::Components
{
    class API_ENGINE ModelComponent: public IComponent
    {
    public:
        void foo() override {}

        ModelComponent(const std::string& p_name)
        {
            m_model = Managers::ModelManager::FindModel(p_name);
        }

        ModelComponent(const std::string& p_file, const std::string& p_name)
        {
            Managers::ModelManager::AddModel(p_file, p_name);
            m_model = Managers::ModelManager::FindModel(p_name);
        }

        uint32_t m_model;
    };
}
