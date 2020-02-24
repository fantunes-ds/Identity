#include <stdafx.h>
#include <Containers/TransformContainer.h>
#include <3DLoader/ObjectElements/Transform.h>
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

int32_t Engine::Containers::TransformContainer::AddTransform()
{
    ObjectElements::Transform transform{};
    GetInstance()->m_transforms.insert_or_assign(transform.GetID(), std::make_shared<ObjectElements::Transform>(transform));

    return transform.GetID();
}

int32_t Engine::Containers::TransformContainer::AddTransform(ObjectElements::Transform& p_transform)
{
    //TODO: Fix this stupidity
    Matrix4F& matrix = p_transform.GetTransformMatrix();
    GetInstance()->m_transforms.insert_or_assign(p_transform.GetID(), std::make_shared<ObjectElements::Transform>(p_transform));
    FindTransform(p_transform.GetID())->SetTransformMatrix(matrix);

    return p_transform.GetID();
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Containers::TransformContainer::FindTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::Containers::TransformContainer::GetTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}
