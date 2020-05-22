#pragma once
#include <Export.h>
#include <list>
#include <Scene/SceneGraph/SceneGraph.h>
#include <Objects/GameObject.h>
#include <Components/Camera.h>

// namespace Engine::Objects
// {
//     class GameObject;
// }

namespace Engine::Components
{
    class IComponent;
}

namespace Engine::Scene
{
    class API_ENGINE Scene : public Objects::IObject
    {
    public:
        Scene() = default;
        Scene(const std::string& p_name);
        ~Scene();

        void SetActiveCamera(std::shared_ptr<Components::Camera> p_camera) { m_activeCamera = p_camera; }
        std::shared_ptr<Components::Camera> GetActiveCamera() { return m_activeCamera; }

        void AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject);
        void RemoveGameObject(std::shared_ptr<Objects::GameObject> p_gameObject);
        void RemoveGameObject(int32_t p_id);

        std::shared_ptr<Objects::GameObject> GetGameObject(const std::string& p_name);

        SceneGraph& GetSceneGraph() { return m_sceneGraph; }
        std::list<std::shared_ptr<Objects::GameObject>> GetAllGameObjectsInScene();
        void SetActiveOnAll(bool p_active);
        void Save();
        void Load(const std::string& p_sceneName);

    private:
        std::shared_ptr<Components::Camera> m_activeCamera;
        SceneGraph m_sceneGraph;
    };
}
