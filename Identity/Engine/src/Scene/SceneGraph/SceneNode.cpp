#include <stdafx.h>

#include <Objects/GameObject.h>
#include <Scene/SceneGraph/SceneNode.h>
#include <Systems/TransformSystem.h>

#include <Managers/SceneManager.h>
#include <Scene/Scene.h>

using namespace Engine::Scene;

SceneNode::SceneNode(std::shared_ptr<Objects::GameObject> p_gameObject) : m_gameObject{ p_gameObject }
{
}

Engine::Scene::SceneNode::SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh) : m_mesh{ p_mesh } {}

void Engine::Scene::SceneNode::AddChild(std::shared_ptr<SceneNode> p_child)
{
    if (!p_child)
        return;
	
    p_child->SetGameObject(std::make_shared<Objects::GameObject>());
    p_child->SetTransform(Systems::TransformSystem::AddTransform());
	
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
    if (!p_child)
        return;
	
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
    auto transform = Systems::TransformSystem::FindTransform(m_transform);

    if (m_parent)
    {
        auto parentTransform = Systems::TransformSystem::FindTransform(m_parent->GetTransform());
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

std::vector<std::shared_ptr<SceneNode>> SceneNode::GetAllChildren()
{
    std::vector<std::shared_ptr<SceneNode>> children;

    for (auto& node : m_children)
    {
        children.emplace_back(node);

        if (node->GetChildren().size() > 0)
        {
            std::vector<std::shared_ptr<SceneNode>> nodeChildren = node->GetAllChildren();
            children.insert(children.end(), nodeChildren.begin(), nodeChildren.end());
        }
    }

    return std::move(children);
}


