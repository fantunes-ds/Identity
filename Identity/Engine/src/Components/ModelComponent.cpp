#include <stdafx.h>

#include <Components/ModelComponent.h>
#include <Objects/GameObject.h>

Engine::Components::ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const int32_t p_id): IComponent{p_gameObject, MODEL}
{
    m_model = p_id;
    m_material = Managers::ResourceManager::GetMaterial("default");
}


Engine::Components::ModelComponent::ModelComponent(Objects::GameObject* p_gameObject): IComponent{p_gameObject, MODEL}, m_model{0}
{
}

Engine::Components::ModelComponent::ModelComponent(Objects::GameObject*            p_gameObject,
                                                   std::shared_ptr<ModelComponent> p_other) : IComponent{p_gameObject, MODEL}
{
    m_model = p_other->m_model;
    m_material = p_other->m_material;
}

Engine::Components::ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_name): IComponent{p_gameObject, MODEL}
{
    m_model = Managers::ResourceManager::GetModel(p_name);
    m_material = Managers::ResourceManager::GetMaterial("default");
}

Engine::Components::ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_file,
                               const std::string& p_name): IComponent{p_gameObject, MODEL}
{
    m_model = Managers::ResourceManager::AddModel(p_file, p_name);
    m_material = Managers::ResourceManager::GetMaterial("default");
}

void Engine::Components::ModelComponent::Serialize(std::ostream& p_stream)
{
    p_stream << typeid(*this).name() << " " << std::to_string(m_id) << "\n{\n" <<
        "   m_model " << Managers::ResourceManager::FindModel(m_model)->GetName() << "\n   {\n" <<
        "       m_path " << Managers::ResourceManager::FindModel(m_model)->GetPath() << "\n" <<
        "   }\n}\n";
}

void Engine::Components::ModelComponent::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
{
    m_gameObject = p_gameObject;

    std::vector <std::string> words;
    std::string modelName;

    for (auto& line : p_block)
    {
        std::stringstream stringStream(line);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (words[0] == "m_model")
        {
            modelName = words[1];
        }
        if (words[0] == "m_path")
        {
            m_model = Managers::ResourceManager::AddModel(words[1], modelName);
        }

        words.clear();
    }
}

bool Engine::Components::ModelComponent::operator==(IComponent* p_other)
{
    if (ModelComponent* other = dynamic_cast<ModelComponent*>(p_other))
    {
        auto thisModel = Managers::ResourceManager::FindModel(m_model);
        auto otherModel = Managers::ResourceManager::FindModel(other->m_model);

        if (thisModel == nullptr || otherModel == nullptr)
            return false;

        auto thisMeshes = thisModel->GetMeshes();
        auto otherMeshes = otherModel->GetMeshes();

        if (thisMeshes.size() != otherMeshes.size())
            return false;

        for (size_t i = 0; i < otherMeshes.size(); ++i)
        {
            if (thisMeshes[i] != otherMeshes[i])
                return false;
        }

        return true;
    }

    return false;
}

bool Engine::Components::ModelComponent::DeleteFromMemory()
{
    return Managers::ResourceManager::RemoveModel(m_model);
}
