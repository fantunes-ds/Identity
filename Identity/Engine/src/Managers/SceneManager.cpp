#include <stdafx.h>
#include <windows.h>
#include <Managers/SceneManager.h>
#include <Scene/Scene.h>

std::unique_ptr<Engine::Managers::SceneManager>& Engine::Managers::SceneManager::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = std::make_unique<SceneManager>();

	return m_instance;
}

std::shared_ptr<Engine::Scene::Scene> Engine::Managers::SceneManager::GetScene(const std::string& p_name)
{
	for (auto& scene : GetInstance()->m_scenes)
	{
		if (scene->GetName() == p_name)
			return scene;
	}

	return nullptr;
}

void Engine::Managers::SceneManager::AddScene(const std::shared_ptr<Scene::Scene> p_scene)
{
	GetInstance()->m_scenes.push_back(p_scene);
}

void Engine::Managers::SceneManager::SetActiveScene(const std::string& p_name)
{
	for (auto scene : GetInstance()->m_scenes)
	{
		if (scene->GetName() == p_name)
		{
			GetInstance()->m_activeScene = scene;
			return;
		}
	}
	
	const std::string info("SceneManager::SetActiveScene(const std::string& p_name): Could not set active scene to " + p_name + " because a scene with this name could not be found\n");
	MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

void Engine::Managers::SceneManager::SetActiveScene(const int32_t p_id)
{
	for (auto scene : GetInstance()->m_scenes)
	{
		if (scene->GetID() == p_id)
		{
			GetInstance()->m_activeScene = scene;
			return;
		}
	}

	const std::string info("SceneManager::SetActiveScene(const int32_t p_id): Could not set active scene to " + std::to_string(p_id) + " because a scene with this name could not be found\n");
	MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

void Engine::Managers::SceneManager::SetPlayScene(const std::string& p_name)
{
	for (auto scene : GetInstance()->m_scenes)
	{
		if (scene->GetName() == p_name)
		{
			GetInstance()->m_playScene = scene;
			return;
		}
	}

	const std::string info("SceneManager::SetPlayScene(const std::string& p_name): Could not set active scene to " + p_name + " because a scene with this name could not be found\n");
	MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}

void Engine::Managers::SceneManager::SetPlayScene(const int32_t p_id)
{
	for (auto scene : GetInstance()->m_scenes)
	{
		if (scene->GetID() == p_id)
		{
			GetInstance()->m_playScene = scene;
			return;
		}
	}

	const std::string info("SceneManager::SetPlayScene(const int32_t p_id): Could not set active scene to " + std::to_string(p_id) + " because a scene with this name could not be found\n");
	MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
}
