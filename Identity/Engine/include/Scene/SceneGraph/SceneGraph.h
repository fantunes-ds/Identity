#pragma once
#include <Export.h>

namespace Engine::Scene
{
    class SceneNode;

    class API_ENGINE SceneGraph
    {
    public:
        SceneGraph() = default;
        ~SceneGraph() = default;

        void AddRootSceneNode(SceneNode& p_sceneNode);
        void UpdateScene(float p_deltaTime);

        const std::vector<SceneNode>& GetRootSceneNodes() const { return m_rootSceneNodes; }

    private:
        std::vector<SceneNode> m_rootSceneNodes;

    };
}

