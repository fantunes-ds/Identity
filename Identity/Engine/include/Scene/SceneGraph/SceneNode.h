#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::ObjectElements
{
    class Mesh;
}

namespace Engine::Objects
{
    class GameObject;
}

namespace Engine::Scene
{
    class API_ENGINE SceneNode : public Objects::IObject
    {
    public:
        SceneNode() = default;
        SceneNode(std::shared_ptr<Objects::GameObject> p_gameObject);
        SceneNode(std::shared_ptr<ObjectElements::Mesh> p_mesh);
        ~SceneNode() = default;

        /***
         * @brief Adds a child node to this SceneNode.
         */
        void AddChild(std::shared_ptr<SceneNode> p_child);
        void RemoveChild(int32_t p_id);
        void RemoveChild(std::shared_ptr<SceneNode> p_child);

        void Update(const float p_deltaTime);
        bool IsRoot();

        [[nodiscard]] const std::vector<std::shared_ptr<SceneNode>>& GetChildren() const { return m_children; }
        [[nodiscard]] std::shared_ptr<ObjectElements::Mesh> GetMesh() const { return m_mesh; }
        [[nodiscard]] int32_t GetTransform() const { return m_transform; }
        [[nodiscard]] std::shared_ptr<Objects::GameObject> GetGameObject() const { return m_gameObject; }
    	SceneNode* GetParentNode() const {return m_parent;}
        void SetTransform(int32_t p_trans) { m_transform = p_trans; }
        void SetParent(SceneNode* p_parent) { m_parent = p_parent; }
        void SetMesh(std::shared_ptr<ObjectElements::Mesh> p_mesh) { m_mesh = p_mesh; }
        void SetGameObject(std::shared_ptr<Objects::GameObject> p_gameObject) { m_gameObject = p_gameObject; }

    protected:
        SceneNode* m_parent = nullptr;
        std::vector<std::shared_ptr<SceneNode>> m_children;
        std::shared_ptr<ObjectElements::Mesh> m_mesh = nullptr;
        std::shared_ptr<Objects::GameObject> m_gameObject = nullptr;
        //TODO: make this into a shared_ptr<Transform>
        int32_t m_transform = -1;
    };
}
