#include <stdafx.h>
#include <Scene/SceneGraph/SceneGraph.h>

void Engine::Scene::SceneGraph::AddRootSceneNode(std::shared_ptr<SceneNode> p_sceneNode)
{
    m_rootSceneNodes.insert_or_assign(p_sceneNode->GetID(), p_sceneNode);
}

void Engine::Scene::SceneGraph::RemoveRootSceneNode(int32_t p_id)
{
    m_rootSceneNodes.erase(p_id);
}

void Engine::Scene::SceneGraph::UpdateScene(float p_deltaTime)
{
    for (auto& node : m_rootSceneNodes)
        node.second->Update(p_deltaTime);
}
