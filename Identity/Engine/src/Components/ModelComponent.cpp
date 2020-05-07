#include <stdafx.h>

#include <Components/ModelComponent.h>
#include <Objects/GameObject.h>

using namespace Engine::Components;

ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const int32_t p_id): IComponent{p_gameObject, MODEL}
{
    m_model = p_id;
}


ModelComponent::ModelComponent(Objects::GameObject*            p_gameObject,
                               std::shared_ptr<ModelComponent> p_other) : IComponent{p_gameObject, MODEL}
{
    m_model = p_other->m_model;
}

ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_name): IComponent{p_gameObject, MODEL}
{
    m_model = Managers::ResourceManager::GetModel(p_name);
}

ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_file, 
                               const std::string& p_name): IComponent{p_gameObject, MODEL}
{
    m_model = Managers::ResourceManager::AddModel(p_file, p_name);
}

bool ModelComponent::operator==(IComponent* p_other)
{
    if (ModelComponent* other = dynamic_cast<ModelComponent*>(p_other))
    {
        auto thisModel  = Managers::ResourceManager::FindModel(m_model)->GetMeshes();
        auto otherModel = Managers::ResourceManager::FindModel(other->m_model)->GetMeshes();

        if (thisModel.size() != otherModel.size())
            return false;

        for (size_t i = 0; i < otherModel.size(); ++i)
        {
            if (thisModel[i] != otherModel[i])
                return false;
        }

        return true;
    }

    return false;
}

bool ModelComponent::DeleteFromMemory()
{
    return Managers::ResourceManager::RemoveModel(m_model);
}
