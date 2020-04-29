#pragma once
#include <Export.h>
#include <Scene/SceneGraph/SceneNode.h>

namespace Engine::Objects
{
    class GameObject;
}

namespace Engine::Scene
{
    /**
     * @brief Class that contains all root SceneNodes of objects present in the Scene. Is currently used to draw the Scene as well.
     */
    class API_ENGINE SceneGraph
    {
    public:
        SceneGraph() = default;
        ~SceneGraph() = default;

        void AddRootSceneNode(std::shared_ptr<SceneNode> p_sceneNode);
        void AddGameObjectToScene(std::shared_ptr<Objects::GameObject> p_gameObject);
        void RemoveRootSceneNode(int32_t p_id);

        void UpdateScene(const float p_deltaTime);

        const std::map<int32_t, std::shared_ptr<SceneNode>>& GetRootSceneNodes() const { return m_rootSceneNodes; }

    private:
        std::map<int32_t, std::shared_ptr<SceneNode>> m_rootSceneNodes;
    };
}

