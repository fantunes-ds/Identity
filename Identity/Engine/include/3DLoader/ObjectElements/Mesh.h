#pragma once
#include <Export.h>
#include <vector>
#include <Geometry/Vertex.h>

namespace Engine::ObjectElements
{
    class API_ENGINE Mesh
    {
    public:
        Mesh() = default;
        Mesh(std::vector<Geometry::Vertex>& p_vertices, std::vector<uint32_t>& p_indices);
        ~Mesh() = default;
        Mesh(const Mesh& p_other);


        std::vector<Geometry::Vertex> m_vertices;
        std::vector<uint32_t> m_indices;
    };
}
