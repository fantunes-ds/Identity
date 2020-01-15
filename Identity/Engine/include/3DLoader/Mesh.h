#pragma once
#include <Export.h>
#include <vector>
#include <Geometry/Vertex.h>

namespace Engine
{
	class API_ENGINE Mesh
	{
	public:
		Mesh(std::vector<Geometry::Vertex>& p_vertices, std::vector<uint32_t>& p_indices);
		Mesh() = default;
		
		std::vector<Geometry::Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};
}
