#pragma once
#include <Export.h>

namespace Engine::ObjectElements
{
    class Mesh;
}

namespace Engine::Scene
{
    class API_ENGINE SceneNode
    {
    public:
        SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh);
        ~SceneNode() = default;

        void AddChild(SceneNode* p_child);
        void Update(float p_deltaTime);


    protected:
        SceneNode* m_parent;
        std::vector<SceneNode*> m_children;
        std::shared_ptr<ObjectElements::Mesh> m_mesh;
        GPM::Matrix4F m_localTransform;
        GPM::Matrix4F m_worldTransform;
    };
}
