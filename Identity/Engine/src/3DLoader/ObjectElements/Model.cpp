#include <stdafx.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectElements/Mesh.h>
#include <3DLoader/ObjectLoader.h>

void Engine::ObjectElements::Model::AddMesh(std::shared_ptr<Engine::ObjectElements::Mesh> p_mesh)
{
    m_meshes.emplace_back(p_mesh);
}

bool Engine::ObjectElements::Model::operator==(const Model& p_other)
{
    if (m_meshes.size() != p_other.m_meshes.size())
        return false;

    for (int i = 0; i < m_meshes.size(); ++i)
    {
        if (*m_meshes[i] != *p_other.m_meshes[i])
            return false;
    }
    return true;
}
