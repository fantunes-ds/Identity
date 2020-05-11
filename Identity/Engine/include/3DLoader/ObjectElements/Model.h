#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Mesh.h>

namespace Engine::Scene
{
    class SceneNode;
}

namespace Engine::ObjectElements
{
    class API_ENGINE Model : public Objects::IObject, public Serialize::ISerializeable
    {
    public:
        Model() = default;
        ~Model() = default;

        /**
         * @brief Add a mesh to the vector of mesh for the selected model
         * @param p_mesh The mesh to add to the vector of mesh
         */
        void AddMesh(const std::shared_ptr<Mesh>& p_mesh);

        inline std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }
        //inline std::shared_ptr<Scene::SceneNode> GetRootNode() { return m_rootNode; }
        inline void SetPath(const std::string& p_path) { m_path = p_path; }
        [[nodiscard]] inline const std::string& GetPath() const { return m_path; }
        //inline void SetRootNode(std::shared_ptr<Scene::SceneNode> p_rootNode) { m_rootNode = p_rootNode; }

        void Serialize(std::ostream& p_stream) override;
        void Unserialize(std::istream& p_stream) override {}

        bool operator==(const Model& p_other);
        bool operator!=(const Model& p_other);

    private:
        std::string m_path{};
        //std::shared_ptr<Scene::SceneNode> m_rootNode;
        std::vector<std::shared_ptr<Mesh>> m_meshes;
    };
}
