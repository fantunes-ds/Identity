#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::ObjectElements
{
    class Mesh;
}

namespace Engine::Scene
{
    class API_ENGINE SceneNode : public Objects::IObject
    {
    public:
        SceneNode() = default;
        SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh);
        ~SceneNode();

        void AddChild(std::shared_ptr<SceneNode> p_child);
        void Update(float p_deltaTime);
        bool IsRoot();

        const std::vector<std::shared_ptr<SceneNode>>& GetChildren() const { return m_children; }
        const std::shared_ptr<ObjectElements::Mesh> GetMesh() const { return m_mesh; }
        const int32_t GetTransform() const { return m_transform; }

        void SetTransform(int32_t p_trans) { m_transform = p_trans; }
        void SetMesh(std::shared_ptr<ObjectElements::Mesh> p_mesh) { m_mesh = p_mesh; }

    protected:
        SceneNode* m_parent = nullptr;
        std::vector<std::shared_ptr<SceneNode>> m_children;
        std::shared_ptr<ObjectElements::Mesh> m_mesh = nullptr;
        int32_t m_transform = -1;
    };
}
