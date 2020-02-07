#include <stdafx.h>
#include <Containers/ModelContainer.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectLoader.h>

Engine::Containers::ModelContainer::~ModelContainer()
{
    delete m_instance;
}

Engine::Containers::ModelContainer* Engine::Containers::ModelContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ModelContainer();
    }

    return m_instance;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Containers::ModelContainer::AddModel(const std::string& p_path, const std::string& p_name)
{
    ModelContainer* ModelContainer = GetInstance();

    if (!ModelContainer->m_graphicsDevice)
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
        mesh->GenerateBuffers(ModelContainer->m_graphicsDevice);

    for (auto& existingModel: ModelContainer->m_models)
    {
        if (*existingModel.second == *model)
        {
            const std::string error("ModelManager::AddModel(const std::string& p_path, const std::string& p_name): Did not load Model at " + p_path + " because it already has been loaded");
            MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
            return nullptr;
        }
    }

    ModelContainer->m_models.insert_or_assign(model->GetID(), model);
    return model;
}

bool Engine::Containers::ModelContainer::RemoveModel(int32_t p_id)
{
    size_t sizeBefore = GetInstance()->m_models.size();
    GetInstance()->m_models.erase(p_id);
    size_t sizeAfter = GetInstance()->m_models.size();

    if (sizeBefore == sizeAfter)
        return false;

    return true;
}

int32_t Engine::Containers::ModelContainer::FindModel(const std::string& p_name)
{
    for (const auto& model: GetInstance()->GetAllModels())
    {
        if (model.second->GetName() == p_name)
            return model.first;
    }

    return -1;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Containers::ModelContainer::FindModel(uint32_t p_id)
{
    return GetInstance()->GetAllModels().at(p_id);
}

