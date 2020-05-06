#include <stdafx.h>
#include <fstream>
#include <Objects/GameObject.h>
#include <Scene/Scene.h>
#include <Scene/SceneGraph/SceneNode.h>

#include "Systems/TransformSystem.h"

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

void Engine::Scene::Scene::Save()
{
    std::ofstream outfile("SerializeTest.txt");

    for (auto& node: m_sceneGraph.GetAllSceneNodes())
    {
        node.second->GetGameObject()->Serialize(outfile);
    }

    outfile.close();
}

void Engine::Scene::Scene::Load(const std::string& p_sceneName)
{
    std::ifstream inFile(p_sceneName);

    std::vector <std::string> block;
    std::vector <std::string> lines;
    auto instance = Systems::TransformSystem::GetInstance();
    for (std::string line; std::getline(inFile, line); )
    {
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); ++i)
    {

        if (lines[i] == "GAMEOBJECT")
        {
            auto go = std::make_shared<Objects::GameObject>();

            while (lines[i] != ";")
            {
                block.push_back(lines[i]);
                ++i;
            }

            //send stream of block
            go->Deserialize(block);
            m_sceneGraph.AddGameObjectToScene(go);
            block.clear();
        }
    }

    inFile.close();
}
