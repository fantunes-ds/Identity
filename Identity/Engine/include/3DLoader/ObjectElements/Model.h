#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Mesh.h>

namespace Engine::ObjectElements
{
    class API_ENGINE Model : public Objects::IObject
    {
    public:
        Model() = default;
        ~Model() = default;

        void AddMesh(const std::shared_ptr<Mesh>& p_mesh);

        std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }
        std::string& GetName() { return m_name; }

        bool operator==(const Model& p_other);
        bool operator!=(const Model& p_other);

    private:
        std::vector<std::shared_ptr<Mesh>> m_meshes;
    };
}
