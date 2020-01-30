#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Mesh.h>

namespace Engine::ObjectElements
{
    class API_ENGINE Model
    {
    public:
        Model() = default;
        ~Model() = default;

        void AddMesh(std::shared_ptr<Mesh> p_mesh);

        std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }
        std::string& GetName() { return m_name; }

        void SetName(const std::string& p_name) { m_name = p_name; }

        bool operator==(const Model& p_other);

    private:
        std::string m_name;
        std::vector<std::shared_ptr<Mesh>> m_meshes;
    };
}
