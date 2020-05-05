#include <stdafx.h>

#include <Objects/GameObject.h>
#include <Scene/Scene.h>
#include <Scene/SceneGraph/SceneNode.h>

Engine::Scene::Scene::Scene(const std::string& p_name)
{
    SetName(p_name);
}


Engine::Scene::Scene::~Scene()
{
    for (auto go : GetAllGameObjectsInScene())
    {
        go.reset();
    }
}

void Engine::Scene::Scene::AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    m_sceneGraph.AddGameObjectToScene(p_gameObject);
}

void Engine::Scene::Scene::RemoveGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    m_sceneGraph.RemoveGameObjectFromScene(p_gameObject);
}

void Engine::Scene::Scene::RemoveGameObject(int32_t p_id)
{
    //for (auto& node: m_sceneGraph.)
}

std::list<std::shared_ptr<Engine::Objects::GameObject>> Engine::Scene::Scene::GetAllGameObjectsInScene()
{
    std::list<std::shared_ptr<Objects::GameObject>> GOs;

    for (auto& node : m_sceneGraph.GetRootSceneNodes())
    {
        GOs.emplace_back(node.second->GetGameObject());
    }

    return GOs;
}

void Engine::Scene::Scene::SetActiveOnAll(bool p_active)
{
    for (auto& go : GetAllGameObjectsInScene())
    {
        go->SetActive(p_active);
    }
}

void Engine::Scene::Scene::Serialize(std::ostream& p_stream)
{
    for (auto& node: m_sceneGraph.GetAllSceneNodes())
    {
        node.second->GetGameObject()->Serialize(p_stream);
    }
}
