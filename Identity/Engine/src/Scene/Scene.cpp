#include <stdafx.h>

#include <Objects/GameObject.h>
#include <Scene/Scene.h>
#include <Scene/SceneGraph/SceneNode.h>

using namespace Engine::Scene;

Scene::Scene(const std::string& p_name)
{
    SetName(p_name);
}


Scene::~Scene()
{
    for (auto go : GetAllGameObjectsInScene())
    {
        go.reset();
    }
}

void Scene::AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    m_sceneGraph.AddGameObjectToScene(p_gameObject);
}

void Scene::RemoveGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
    m_sceneGraph.RemoveGameObjectFromScene(p_gameObject);
}

void Scene::RemoveGameObject(int32_t p_id)
{
    //for (auto& node: m_sceneGraph.)
}

std::list<std::shared_ptr<Engine::Objects::GameObject>> Scene::GetAllGameObjectsInScene()
{
    std::list<std::shared_ptr<Objects::GameObject>> GOs;

    for (auto& node : m_sceneGraph.GetRootSceneNodes())
    {
        GOs.emplace_back(node.second->GetGameObject());
    }

    return GOs;
}

void Scene::SetActiveOnAll(bool p_active)
{
    for (auto& go : GetAllGameObjectsInScene())
    {
        go->SetActive(p_active);
    }
}
