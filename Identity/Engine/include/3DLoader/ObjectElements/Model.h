#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Mesh.h>

namespace Engine::Scene
{
    class SceneNode;
}

namespace Engine::ObjectElements
{
    class API_ENGINE Model : public Objects::IObject
    {
    public:
        Model() = default;
        ~Model() = default;

        void AddMesh(const std::shared_ptr<Mesh>& p_mesh);

        inline std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }
        inline std::string& GetName() { return m_name; }
        inline const std::shared_ptr<Scene::SceneNode> GetRootNode() const { return m_rootNode; }

        inline void SetRootNode(std::shared_ptr<Scene::SceneNode> p_rootNode) { m_rootNode = p_rootNode; }

        bool operator==(const Model& p_other);
        bool operator!=(const Model& p_other);

    private:

        std::shared_ptr<Scene::SceneNode> m_rootNode;
        std::vector<std::shared_ptr<Mesh>> m_meshes;
    };
}
