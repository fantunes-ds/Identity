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

        void GenerateBuffers(Microsoft::WRL::ComPtr<ID3D11Device> p_device);
        void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context);

        Rendering::Buffers::VertexBuffer m_vertexBuffer;
        Rendering::Buffers::IndexBuffer m_indexBuffer;
        std::vector<Geometry::Vertex> m_vertices;
        std::vector<unsigned short> m_indices;
    };
}
