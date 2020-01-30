#pragma once
#include <Export.h>
#include <vector>
#include <Geometry/Vertex.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <Rendering/Buffers/VertexBuffer.h>
#include <Rendering/Buffers/IndexBuffer.h>

namespace Engine::ObjectElements
{
    class API_ENGINE Mesh
    {
    public:
        Mesh() = default;
        Mesh(std::vector<Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices);
        ~Mesh() = default;
        Mesh(const Mesh& p_other);

        void GenerateBuffers(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device);
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        bool operator==(const Mesh& p_other) const;
        bool operator!=(const Mesh& p_other) const;

        Rendering::Buffers::VertexBuffer& GetVertexBuffer() { return m_vertexBuffer; }
        Rendering::Buffers::IndexBuffer& GetIndexBuffer() { return m_indexBuffer; }
        std::vector<Geometry::Vertex>& GetVertices() { return m_vertices; }
        std::vector<unsigned short>& GetIndices() { return m_indices; }

    private:
        Rendering::Buffers::VertexBuffer m_vertexBuffer;
        Rendering::Buffers::IndexBuffer m_indexBuffer;
        std::vector<Geometry::Vertex> m_vertices;
        std::vector<unsigned short> m_indices;
    };
}
