#pragma once
#include <Export.h>
#include <Scene/SceneGraph/SceneGraph.h>

namespace Engine::Objects
{
    class GameObject;
}

namespace Engine::Scene
{
    class API_ENGINE Scene: public Objects::IObject
    {
    public:
        Scene() = default;
        ~Scene() = default;

        void AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject);
        SceneGraph& GetSceneGraph() { return m_sceneGraph; }
    	
    private:
        SceneGraph m_sceneGraph;
    };
}
