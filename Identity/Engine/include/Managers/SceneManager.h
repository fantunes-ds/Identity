#pragma once
#include <Export.h>
#include <memory>
#include <vector>

namespace Engine::Scene
{
	class Scene;
}

namespace Engine::Managers
{
	class API_ENGINE SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager() = default;
		SceneManager(const SceneManager&) = delete;
		SceneManager(const SceneManager&&) = delete;

		static std::unique_ptr<SceneManager>& GetInstance();
		static void AddScene(const std::shared_ptr<Scene::Scene> p_scene);
		static std::vector<std::shared_ptr<Scene::Scene>>& GetScenes() { return GetInstance()->m_scenes; }
		
		static void SetActiveScene(const std::string& p_name);
		static void SetActiveScene(const int32_t p_id);
		static void SetActiveScene(std::shared_ptr<Scene::Scene> p_scene) { GetInstance()->m_activeScene = p_scene; }

		static std::shared_ptr<Scene::Scene> GetActiveScene() { return GetInstance()->m_activeScene; }
		
	private:
		inline static std::unique_ptr<SceneManager> m_instance{ nullptr };
		std::vector<std::shared_ptr<Scene::Scene>> m_scenes;
		std::shared_ptr<Scene::Scene> m_activeScene{nullptr};
	};
}
