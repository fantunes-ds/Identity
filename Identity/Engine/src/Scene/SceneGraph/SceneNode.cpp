#include <stdafx.h>
#include <Scene/SceneGraph/SceneNode.h>
#include "Containers/TransformContainer.h"

Engine::Scene::SceneNode::SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh) : m_mesh{ p_mesh } {}

Engine::Scene::SceneNode::~SceneNode()
{
}

void Engine::Scene::SceneNode::AddChild(std::shared_ptr<SceneNode> p_child)
{
    p_child->SetTransform(Containers::TransformContainer::AddTransform());

    m_children.push_back(p_child);
    p_child->m_parent = this;
}

void Engine::Scene::SceneNode::Update(float p_deltaTime)
{
    auto transform = Containers::TransformContainer::FindTransform(m_transform);

    if (m_parent)
    {
        auto parentTransform = Containers::TransformContainer::FindTransform(m_parent->GetTransform());
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


