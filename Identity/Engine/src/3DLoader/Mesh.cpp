#include <stdafx.h>

#include <3DLoader/Mesh.h>

Engine::Mesh::Mesh(std::vector<Geometry::Vertex>& p_vertices, std::vector<uint32_t>& p_indices) : m_vertices{ p_vertices }, m_indices{ p_indices }
{
}
