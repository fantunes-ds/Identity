#include <stdafx.h>
#include <Containers/TransformContainer.h>
#include <Tools/IDCounter.h>

Engine::Containers::TransformContainer::~TransformContainer()
{
    delete m_instance;
}

Engine::Containers::TransformContainer* Engine::Containers::TransformContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new TransformContainer();
    }

    return m_instance;
}

int32_t Engine::Containers::TransformContainer::AddTransform(ObjectElements::Transform& p_transform)
{
    uint32_t id = Tools::IDCounter::GetNewID();

    //TODO: Fix this stupidity
    Matrix4F& matrix = p_transform.GetTransformMatrix();
    GetInstance()->m_transforms.insert_or_assign(id, std::make_shared<ObjectElements::Transform>(p_transform));
    FindTransform(id)->SetTransformMatrix(matrix);

    return id;
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Containers::TransformContainer::FindTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Containers::TransformContainer::GetTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}
