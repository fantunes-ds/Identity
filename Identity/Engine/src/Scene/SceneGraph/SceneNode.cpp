#include <stdafx.h>
#include <Scene/SceneGraph/SceneNode.h>

Engine::Scene::SceneNode::SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh) : m_mesh { p_mesh }, m_parent { nullptr } {}

void Engine::Scene::SceneNode::AddChild(SceneNode* p_child)
{
    m_children.push_back(p_child);
    p_child->m_parent = this;
}

void Engine::Scene::SceneNode::Update(float p_deltaTime)
{
    if (m_parent)
        m_worldTransform = m_parent->m_worldTransform * m_localTransform;
    else
        m_worldTransform = m_localTransform;

    for (auto child: m_children)
    {
        child->Update(p_deltaTime);
    }

}


