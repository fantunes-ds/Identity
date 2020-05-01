#pragma once
#include <Export.h>
#include <list>
#include <Scene/SceneGraph/SceneGraph.h>

namespace Engine::Objects
{
    class GameObject;
}

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

        void AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject);

        SceneGraph& GetSceneGraph() { return m_sceneGraph; }
        std::list<std::shared_ptr<Objects::GameObject>> GetAllGameObjectsInScene();
        void SetActiveOnAll(bool p_active);
    private:
        SceneGraph m_sceneGraph;
    };
}
