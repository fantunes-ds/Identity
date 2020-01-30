#include <stdafx.h>
#include <Managers/ModelManager.h>
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

std::shared_ptr<Engine::ObjectElements::Model> Engine::Manager::ModelManager::AddModel(const std::string& p_path, const std::string& p_name)
{
    if (!m_graphicsDevice)
    {
        std::string error("ModelManager::AddModel(const std::string& p_path, const std::string& p_name): Could not load model at " + p_path + " because ModelManager was not assigned a Graphics Device");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    std::shared_ptr<ObjectElements::Model> model = ObjectLoader::LoadModel(p_path);
    model->SetName(p_name);

    for (auto& mesh : model->GetMeshes())
        mesh->GenerateBuffers(m_graphicsDevice);

    for (auto existingModel: m_models)
    {
        if (*existingModel == *model)
        {
            std::string error("ModelManager::AddModel(const std::string& p_path, const std::string& p_name): Did not load Model at " + p_path + " because it already has been loaded");
            MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
            return nullptr;
        }
    }

    m_models.emplace_back(model);
    return model;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Manager::ModelManager::FindModel(const std::string& p_name)
{
    for (auto model: m_models)
    {
        if (model->GetName() == p_name)
            return model;
    }

    return nullptr;
}

