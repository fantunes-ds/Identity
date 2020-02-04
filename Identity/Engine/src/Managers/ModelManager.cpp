#include <stdafx.h>
#include <Managers/ModelManager.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectLoader.h>

Engine::Managers::ModelManager::~ModelManager()
{
    delete m_instance;
}

Engine::Managers::ModelManager* Engine::Managers::ModelManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ModelManager();
    }

    return m_instance;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Managers::ModelManager::AddModel(const std::string& p_path, const std::string& p_name)
{
    ModelManager* modelManager = ModelManager::GetInstance();

    if (!modelManager->m_graphicsDevice)
    {
        const std::string error("ModelManager::AddModel(const std::string& p_path, const std::string& p_name): Could not load model at " + p_path + " because ModelManager was not assigned a Graphics Device");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    std::shared_ptr<ObjectElements::Model> model = ObjectLoader::LoadModel(p_path);

    if (model == nullptr)
    {
		const std::string error("ModelManager::AddModel(const std::string& p_path, const std::string& p_name): Could not load model at " + p_path + " because there was no object to be found at that path");
		MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);       
		return nullptr;
    }
    model->SetName(p_name);

    for (auto& mesh : model->GetMeshes())
        mesh->GenerateBuffers(modelManager->m_graphicsDevice);

    for (auto& existingModel: modelManager->m_models)
    {
        if (*existingModel.second == *model)
        {
            const std::string error("ModelManager::AddModel(const std::string& p_path, const std::string& p_name): Did not load Model at " + p_path + " because it already has been loaded");
            MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
            return nullptr;
        }
    }

    modelManager->m_models.insert_or_assign(Tools::IDCounter::GetNewID(), model);
    return model;
}

int32_t Engine::Managers::ModelManager::FindModel(const std::string& p_name)
{
    for (const auto& model: GetInstance()->GetAllModels())
    {
        if (model.second->GetName() == p_name)
            return model.first;
    }

    return -1;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Managers::ModelManager::FindModel(uint32_t p_id)
{
    return GetInstance()->GetAllModels().at(p_id);
}

