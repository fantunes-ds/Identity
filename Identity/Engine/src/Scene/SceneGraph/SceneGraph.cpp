#include <stdafx.h>
#include <Scene/SceneGraph/SceneGraph.h>
#include <Objects/GameObject.h>

#include "Managers/ResourceManager.h"

void Engine::Scene::SceneGraph::AddRootSceneNode(std::shared_ptr<SceneNode> p_sceneNode)
{
    m_rootSceneNodes.insert_or_assign(p_sceneNode->GetID(), p_sceneNode);
}

void Engine::Scene::SceneGraph::AddGameObjectToScene(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    int modelID = p_gameObject->FindComponentOfType<Components::ModelComponent>()->GetModel();
    auto model = Managers::ResourceManager::FindModel(modelID);

    auto rootNode = std::make_shared<SceneNode>();

	for (auto& mesh: model->GetMeshes())
	{
        rootNode->AddChild(std::make_shared<SceneNode>(mesh));
	}

    rootNode->SetTransform(p_gameObject->GetTransformID());
    AddRootSceneNode(rootNode);
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
