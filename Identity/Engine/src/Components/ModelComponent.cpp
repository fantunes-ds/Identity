#include <stdafx.h>
#include <Components/ModelComponent.h>

Engine::Components::ModelComponent::ModelComponent(const std::string& p_name)
{
    m_model = Containers::ModelContainer::FindModel(p_name);
}

Engine::Components::ModelComponent::ModelComponent(const std::string& p_file, const std::string& p_name)
{
    Containers::ModelContainer::AddModel(p_file, p_name);
    m_model = Containers::ModelContainer::FindModel(p_name);
}

bool Engine::Components::ModelComponent::operator==(IComponent* p_other)
{
    if (ModelComponent* other = dynamic_cast<ModelComponent*>(p_other))
    {
        auto thisModel = Containers::ModelContainer::FindModel(m_model)->GetMeshes();
        auto otherModel = Containers::ModelContainer::FindModel(other->m_model)->GetMeshes();

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

bool Engine::Components::ModelComponent::DeleteFromMemory()
{
    return Containers::ModelContainer::RemoveModel(m_model);
}
