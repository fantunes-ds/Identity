#include <stdafx.h>

#include <windows.h>

#include <Components/BoxCollider.h>
#include <Managers/SceneManager.h>
#include <Scene/Scene.h>

using namespace Engine::Managers;

std::unique_ptr<SceneManager>& SceneManager::GetInstance()
{
    if (m_instance == nullptr)
        m_instance = std::make_unique<SceneManager>();

    return m_instance;
}

std::shared_ptr<Engine::Scene::Scene> SceneManager::GetScene(const std::string& p_name)
{
    for (auto& scene : GetInstance()->m_scenes)
    {
        if (scene->GetName() == p_name)
            return scene;
    }

    return nullptr;
}

void SceneManager::AddScene(const std::shared_ptr<Scene::Scene> p_scene)
{
    GetInstance()->m_scenes.push_back(p_scene);
}

void SceneManager::SetActiveScene(const std::string& p_name)
{
    for (auto scene : GetInstance()->m_scenes)
    {
        if (scene->GetName() == p_name)
        {
            GetInstance()->m_activeScene = scene;
            return;
        }
    }

    const std::string info("SceneManager::SetActiveScene(const std::string& p_name): Could not set active scene to " +
                           p_name + " because a scene with this name could not be found\n");
    MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

void SceneManager::SetActiveScene(const int32_t p_id)
{
    for (auto scene : GetInstance()->m_scenes)
    {
        if (scene->GetID() == p_id)
        {
            GetInstance()->m_activeScene = scene;
            return;
        }
    }

    const std::string info("SceneManager::SetActiveScene(const int32_t p_id): Could not set active scene to " + std::
                           to_string(p_id) + " because a scene with this name could not be found\n");
    MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

void SceneManager::SetPlayScene(const std::string& p_name)
{
    for (auto scene : GetInstance()->m_scenes)
    {
        if (scene->GetName() == p_name)
        {
            GetInstance()->m_playScene = scene;
            return;
        }
    }

    const std::string info{
        "SceneManager::SetPlayScene(const std::string& p_name): Could not set active scene to " +
        p_name + " because a scene with this name could not be found\n"
    };
    MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

void SceneManager::SetPlayScene(const int32_t p_id)
{
    for (auto scene : GetInstance()->m_scenes)
    {
        if (scene->GetID() == p_id)
        {
            GetInstance()->m_playScene = scene;
            return;
        }
    }

    const std::string info{ "SceneManager::SetPlayScene(const int32_t p_id): Could not set active scene to " + std::
                           to_string(p_id) + " because a scene with this name could not be found\n" };
    MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

bool SceneManager::DeletePlayScene()
{
    return GetInstance()->DeletePlaySceneNS();
}


void SceneManager::DuplicateScene(std::shared_ptr<Scene::Scene>& p_destination, std::shared_ptr<Scene::Scene>& p_source)
{
    for (auto gameObject : p_source->GetAllGameObjectsInScene())
    {
        //create new gameobject
        auto name   = gameObject->GetName() + "1";
        auto tmpOBJ = std::make_shared<Objects::GameObject>(name);
        //we now have gameobject with same name

        //change the transform to be the same
        auto goTransform = gameObject->GetTransform();
        tmpOBJ->GetTransform()->CopyFrom(gameObject->GetTransform());

        //Find all component of each type
        for (auto component : gameObject->FindAllComponentsOfType<Components::BoxCollider>())
        {
            tmpOBJ->AddComponent<Components::BoxCollider>(component);
        }
        for (auto component : gameObject->FindAllComponentsOfType<Components::ModelComponent>())
        {
            tmpOBJ->AddComponent<Components::ModelComponent>(component);
        }

        p_destination->AddGameObject(tmpOBJ);
    }
}

bool SceneManager::DeletePlaySceneNS()
{
    int id = -1;
    for (int i = 0; i < m_scenes.size(); ++i)
    {
        if (m_scenes[i] == m_playScene)
            id = i;
    }

    if (id < 0)
        return false;

    m_scenes[id].reset();
    m_playScene.reset();
    return true;
}
