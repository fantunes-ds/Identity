#include <stdafx.h>
#include <Scene/Scene.h>
#include <Objects/GameObject.h>


Engine::Scene::Scene::Scene(const std::string& p_name)
{
	SetName(p_name);
}

void Engine::Scene::Scene::AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
	m_sceneGraph.AddGameObjectToScene(p_gameObject);
}
