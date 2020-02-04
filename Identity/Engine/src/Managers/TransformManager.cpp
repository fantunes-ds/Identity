#include <stdafx.h>
#include <Managers/TransformManager.h>
#include <Tools/IDCounter.h>

Engine::Managers::TransformManager::~TransformManager()
{
    delete m_instance;
}

Engine::Managers::TransformManager* Engine::Managers::TransformManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new TransformManager();
    }

    return m_instance;
}

int32_t Engine::Managers::TransformManager::AddTransform(ObjectElements::Transform& p_transform)
{
    uint32_t id = Tools::IDCounter::GetNewID();

    //TODO: Fix this stupidity
    Matrix4F& matrix = p_transform.GetTransformMatrix();
    GetInstance()->m_transforms.insert_or_assign(id, std::make_shared<ObjectElements::Transform>(p_transform));
    FindTransform(id)->SetTransformMatrix(matrix);

    return id;
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Managers::TransformManager::FindTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Managers::TransformManager::GetTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}
