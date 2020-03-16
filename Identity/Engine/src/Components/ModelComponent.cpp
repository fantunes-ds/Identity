#include <stdafx.h>
#include <Components/ModelComponent.h>
#include <Scene/SceneGraph/SceneNode.h>
#include <Objects/GameObject.h>

Engine::Components::ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_name): IComponent{ p_gameObject }
{
    m_model = Containers::ModelContainer::FindModel(p_name);
}

Engine::Components::ModelComponent::ModelComponent(Objects::GameObject* p_gameObject, const std::string& p_file, const std::string& p_name): IComponent{ p_gameObject }
{
    Containers::ModelContainer::AddModel(p_file, p_name);
    m_model = Containers::ModelContainer::FindModel(p_name);
    // m_gameObject->SetTransform(Containers::ModelContainer::GetAllModels().at(m_model)->GetRootNode()->GetTransform());
    auto test = Containers::ModelContainer::GetAllModels().at(m_model)->GetRootNode();
    test->SetTransform(m_gameObject->GetTransform()->GetID());
    std::cout << "test";
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
