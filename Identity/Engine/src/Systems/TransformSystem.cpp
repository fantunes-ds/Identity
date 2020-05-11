#include <stdafx.h>
#include <Systems/TransformSystem.h>
#include <Components/Transform.h>

Engine::Systems::TransformSystem::~TransformSystem()
{
    delete m_instance;
}

Engine::Systems::TransformSystem* Engine::Systems::TransformSystem::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new TransformSystem();
    }

    return m_instance;
}

void Engine::Systems::TransformSystem::Update(const float p_deltaTime)
{
    GetInstance()->IUpdate(p_deltaTime);
}

void Engine::Systems::TransformSystem::IUpdate(float p_deltaTime, bool p_isEditor)
{
    for (std::shared_ptr<Engine::Components::Transform> transform : GetTransforms())
    {
        if (transform->IsActive())
        {
            if (transform->needUpdate)
            {
                if (transform->needAxesUpdate)
                    transform->CalculateAxes();

                transform->UpdateWorldTransformMatrix();
            }
        }
    }
}


std::vector<std::shared_ptr<Engine::Components::Transform>> Engine::Systems::TransformSystem::GetTransforms()
{
    std::vector<std::shared_ptr<Engine::Components::Transform>> transforms;

    for (const auto& transform : GetAllTransforms())
    {
        transforms.push_back(transform.second);
    }

    return transforms;
}

int32_t Engine::Systems::TransformSystem::AddTransform()
{
    Components::Transform transform{};
    GetInstance()->m_transforms.insert_or_assign(transform.GetID(), std::make_shared<Components::Transform>(transform));

    return transform.GetID();
}

int32_t Engine::Systems::TransformSystem::AddTransform(std::shared_ptr<Components::Transform> p_transform)
{
    //TODO: Fix this stupidity
    GetInstance()->m_transforms.insert_or_assign(p_transform->GetID(), p_transform);

    return p_transform->GetID();
}

std::shared_ptr<Engine::Components::Transform> Engine::Systems::TransformSystem::FindTransform(uint32_t p_id)
{
    return GetInstance()->m_transforms.at(p_id);
}

std::shared_ptr<Engine::Components::Transform> Engine::Systems::TransformSystem::GetTransform(uint32_t p_id)
{
    auto instance = GetInstance();
    return GetInstance()->m_transforms.at(p_id);
}
