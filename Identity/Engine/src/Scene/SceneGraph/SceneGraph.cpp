#include <stdafx.h>
#include <Scene/SceneGraph/SceneGraph.h>
#include <Objects/GameObject.h>
#include <Managers/ResourceManager.h>

void Engine::Scene::SceneGraph::AddRootSceneNode(std::shared_ptr<SceneNode> p_sceneNode)
{
    m_rootSceneNodes.insert_or_assign(p_sceneNode->GetID(), p_sceneNode);
}

void Engine::Scene::SceneGraph::AddGameObjectToScene(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    auto rootNode = std::make_shared<SceneNode>(p_gameObject);
    rootNode->SetName(p_gameObject->GetName());
    int submeshNumber = 0;

    if (p_gameObject->FindComponentOfType<Components::ModelComponent>())
    {
        int modelID = p_gameObject->FindComponentOfType<Components::ModelComponent>()->GetModel();
        auto model = Managers::ResourceManager::FindModel(modelID);
    	
        for (auto& mesh : model->GetMeshes())
        {
            ++submeshNumber;
            auto child = std::make_shared<SceneNode>(mesh);
            rootNode->AddChild(child);
            child->SetName(rootNode->GetGameObject()->GetName() + "(" + std::to_string(submeshNumber) + ")");
        }
    }
	
    rootNode->SetTransform(p_gameObject->GetTransformID());
    AddRootSceneNode(rootNode);
    p_gameObject->SetSceneNode(rootNode);
}

void Engine::Scene::SceneGraph::RemoveGameObjectFromScene(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    for (auto& node : m_rootSceneNodes)
    {
        if (node.second->GetGameObject() == p_gameObject)
        {
            if (!node.second->GetChildren().empty())
            {
                RemoveGameObjectFromScene(node.second->GetGameObject());
            }

            m_rootSceneNodes.erase(node.first);
        }
    }
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

std::map<int32_t, std::shared_ptr<Engine::Scene::SceneNode>> Engine::Scene::SceneGraph::GetAllSceneNodes()
{
    std::map<int32_t, std::shared_ptr<Engine::Scene::SceneNode>> map;

    //TODO: check childrens' children
    for (auto& node : m_rootSceneNodes)
    {
        map.insert_or_assign(node.first, node.second);
    }

    return map;
}

