#include <stdafx.h>
#include <Scene/Scene.h>
#include <Objects/GameObject.h>


void Engine::Scene::Scene::AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject)
{
	m_sceneGraph.AddGameObjectToScene(p_gameObject);
}
