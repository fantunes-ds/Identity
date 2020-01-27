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
        Mesh(std::vector<Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices);
        ~Mesh() = default;
        Mesh(const Mesh& p_other);


        std::vector<Geometry::Vertex> m_vertices;
        std::vector<unsigned short> m_indices;
    };
}
