#include <stdafx.h>
#include <3DLoader/Manager/ModelManager.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectLoader.h>

Engine::Manager::ModelManager::~ModelManager()
{
    delete m_instance;
}

Engine::Manager::ModelManager* Engine::Manager::ModelManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ModelManager();
    }

    return m_instance;
}

void Engine::Manager::ModelManager::AddModel(const std::string& p_path, const std::string& p_name)
{
    std::shared_ptr<ObjectElements::Model> model = ObjectLoader::LoadModel(p_path);
    model->SetName(p_name);
    m_models.emplace_back(model);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Manager::ModelManager::FindModel(std::string& p_name)
{
    for (auto model: m_models)
    {
        if (model->GetName() == p_name)
            return model;
    }

    return nullptr;
}

