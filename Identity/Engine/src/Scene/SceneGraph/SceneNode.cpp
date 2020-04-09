#include <stdafx.h>
#include <Scene/SceneGraph/SceneNode.h>
#include <Systems/TransformSystem.h>

Engine::Scene::SceneNode::SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh) : m_mesh{ p_mesh } {}

Engine::Scene::SceneNode::~SceneNode()
{
}

void Engine::Scene::SceneNode::AddChild(std::shared_ptr<SceneNode> p_child)
{
    p_child->SetTransform(Containers::TransformSystem::AddTransform());

    m_children.push_back(p_child);
    p_child->m_parent = this;
}

void Engine::Scene::SceneNode::RemoveChild(int32_t p_id)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if (it->get()->GetID() == p_id)
            m_children.erase(it);
    }
}

void Engine::Scene::SceneNode::RemoveChild(std::shared_ptr<SceneNode> p_child)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if (it->get()->GetID() == p_child->GetID())
        {
            m_children.erase(it);
        }
    }
}

void Engine::Scene::SceneNode::Update(float p_deltaTime)
{
    auto transform = Containers::TransformSystem::FindTransform(m_transform);

    if (m_parent)
    {
        auto parentTransform = Containers::TransformSystem::FindTransform(m_parent->GetTransform());
        transform->SetWorldTransformMatrix(parentTransform->GetWorldTransformMatrix() * transform->GetLocalTransformMatrix());
    }
    else
        transform->SetWorldTransformMatrix(transform->GetWorldTransformMatrix());

    for (auto child : m_children)
    {
        child->Update(p_deltaTime);
    }

}

bool Engine::Scene::SceneNode::IsRoot()
{
    if (m_parent)
        return false;

    return true;
}


