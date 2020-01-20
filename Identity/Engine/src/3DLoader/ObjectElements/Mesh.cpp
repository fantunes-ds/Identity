#include <stdafx.h>

#include <3DLoader/ObjectElements/Mesh.h>

Engine::ObjectElements::Mesh::Mesh(std::vector<Engine::Geometry::Vertex>& p_vertices, std::vector<uint32_t>& p_indices) : m_vertices{ p_vertices }, m_indices{ p_indices }
{
}
