#pragma once
#include <Export.h>
#include <Scene/SceneGraph/SceneNode.h>

namespace Engine::Scene
{
    class API_ENGINE SceneGraph
    {
    public:
        SceneGraph() = default;
        ~SceneGraph() = default;

        void AddRootSceneNode(std::shared_ptr<SceneNode> p_sceneNode);
        void RemoveRootSceneNode(int32_t p_id);


        void UpdateScene(float p_deltaTime);

        const std::map<int32_t, std::shared_ptr<SceneNode>>& GetRootSceneNodes() const { return m_rootSceneNodes; }

        static SceneGraph* GetInstance()
        {
            if (m_instance == nullptr)
            {
                m_instance = new SceneGraph();
            }

            return m_instance;
        }
    private:
        std::map<int32_t, std::shared_ptr<SceneNode>> m_rootSceneNodes;
        inline static SceneGraph* m_instance;
    };
}

