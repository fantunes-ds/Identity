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
    p_stream << typeid(*this).name() << "\n{\n" <<
        "   m_model " << Managers::ResourceManager::FindModel(m_model)->GetName() << "\n" <<
        "   m_material " << m_material->GetName() << "\n" <<
        "}\n";
}

void Engine::Components::ModelComponent::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
{
    m_gameObject = p_gameObject;

    std::vector <std::string> words;

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
            m_model = Managers::ResourceManager::GetModel(words[1]);
        }
        else if (words[0] == "m_material")
        {
            m_material = Managers::ResourceManager::GetMaterial(words[1]);
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

void Engine::Components::ModelComponent::SetActive(bool p_active)
{
    m_isActive = p_active;
}


void Engine::Components::ModelComponent::SetMaterial(const std::string& p_name)
{
    if (p_name == "RenderText")
    {
        const std::string info("You cannot use this Material on an object");
        MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
        return;
    }
    m_material = Managers::ResourceManager::GetMaterial(p_name);
}
