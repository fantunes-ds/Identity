#include <stdafx.h>
#include <Objects/GameObject.h>
#include <Managers/ModelManager.h>
#include <Managers/TransformManager.h>

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Objects::GameObject::GetTransform() const
{
    return Managers::TransformManager::GetInstance()->GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Objects::GameObject::GetModel() const
{
    return Managers::ModelManager::GetInstance()->FindModel(m_model);
}

void Engine::Objects::GameObject::SetModel(const std::string& p_name)
{
    int32_t id = Managers::ModelManager::FindModel(p_name);

    if (id < 0)
    {
        const std::string error("GameObject::SetModel(const std::string& p_name): Could not find model with name " + p_name + "\n");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return;
    }

    m_model = id;
}

bool Engine::Objects::GameObject::operator==(GameObject& p_other) const
{
    if (m_model == p_other.m_model && m_transform == p_other.m_transform)
        return true;

    return false;
}
