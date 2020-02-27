#include <stdafx.h>
#include <Scene/SceneGraph/SceneGraph.h>

void Engine::Scene::SceneGraph::AddRootSceneNode(std::shared_ptr<SceneNode> p_sceneNode)
{
    m_rootSceneNodes.emplace_back(p_sceneNode);
}

void Engine::Scene::SceneGraph::UpdateScene(float p_deltaTime)
{
    for (auto& node : m_rootSceneNodes)
        node->Update(p_deltaTime);
}
