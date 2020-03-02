#include <stdafx.h>
#include <Scene/SceneGraph/SceneGraph.h>
#include <Scene/SceneGraph/SceneNode.h>

void Engine::Scene::SceneGraph::AddRootSceneNode(SceneNode& p_sceneNode)
{
    m_rootSceneNodes.emplace_back(std::move(p_sceneNode));
}

void Engine::Scene::SceneGraph::UpdateScene(float p_deltaTime)
{
    for (auto& node : m_rootSceneNodes)
        node.Update(p_deltaTime);
}
