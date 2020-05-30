#include <stdafx.h>

#include <Managers/ResourceManager.h>
#include <Objects/GameObject.h>
#include <Scene/SceneGraph/SceneGraph.h>

#include <Containers/GameObjectContainer.h>

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
        int  modelID = p_gameObject->FindComponentOfType<Components::ModelComponent>()->GetModel();
        auto model = Managers::ResourceManager::FindModel(modelID);

        if (model)
        {
            for (auto& mesh : model->GetMeshes())
            {
                ++submeshNumber;
                auto child = std::make_shared<SceneNode>(mesh);
                rootNode->AddChild(child);
                child->SetName(rootNode->GetGameObject()->GetName() + "(" + std::to_string(submeshNumber) + ")");
                child->GetGameObject()->SetName(rootNode->GetGameObject()->GetName() + "(" + std::to_string(submeshNumber) + ")");
                child->GetGameObject()->SetSceneNode(child);
            }
        }
    }

    rootNode->SetTransform(p_gameObject->GetTransformID());
    AddRootSceneNode(rootNode);
    p_gameObject->SetSceneNode(rootNode);
}

void Engine::Scene::SceneGraph::RemoveGameObjectFromScene(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    if (p_gameObject == nullptr)
        return;

    if (p_gameObject->GetSceneNode())
    {
        if (p_gameObject->GetSceneNode()->IsRoot())
            RemoveRootSceneNode(p_gameObject->GetSceneNode()->GetID());
    }

    Containers::GameObjectContainer::RemoveGameObject(p_gameObject);
}

void Engine::Scene::SceneGraph::RemoveRootSceneNode(int32_t p_id)
{
    /*for (auto child : m_rootSceneNodes.find(p_id)->second->GetAllChildren())
            m_rootSceneNodes.erase(child->GetID());*/

    if (m_rootSceneNodes.find(p_id) != m_rootSceneNodes.end())
            m_rootSceneNodes.erase(p_id);
}

void Engine::Scene::SceneGraph::UpdateScene(float p_deltaTime)
{
    for (auto& node : m_rootSceneNodes)
        node.second->Update(p_deltaTime);
}

std::map<int32_t, std::shared_ptr<Engine::Scene::SceneNode>> Engine::Scene::SceneGraph::GetAllSceneNodes()
{
    std::map<int32_t, std::shared_ptr<SceneNode>> map;

    //TODO: check childrens' children
    for (auto& node : m_rootSceneNodes)
    {
        map.insert_or_assign(node.first, node.second);

        for (auto child: node.second->GetAllChildren())
            map.insert_or_assign(child->GetID(), child);
    }


    return map;
}
