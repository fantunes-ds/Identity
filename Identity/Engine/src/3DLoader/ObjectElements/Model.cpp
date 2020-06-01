#include <stdafx.h>

#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectElements/Mesh.h>
#include <3DLoader/ObjectLoader.h>

using namespace Engine::ObjectElements;

void Model::AddMesh(const std::shared_ptr<Mesh>& p_mesh)
{
    m_meshes.emplace_back(p_mesh);
}

bool Model::operator==(const Model& p_other)
{
    if (m_meshes.size() != p_other.m_meshes.size())
        return false;

    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        if (*m_meshes[i] != *p_other.m_meshes[i])
            return false;
    }
    return true;
}

bool Model::operator!=(const Model& p_other)
{
    if (m_meshes.size() != p_other.m_meshes.size())
        return true;

    for (size_t i = 0; i < m_meshes.size(); ++i)
    {
        if (*m_meshes[i] != *p_other.m_meshes[i])
            return true;
    }
    return false;
}

void Model::Serialize(std::ostream& p_stream)
{
    p_stream << "MODEL " << GetPath() << " " << GetName() << '\n';
}
