#pragma once
#include <Export.h>
#include <vector>
#include <Geometry/Vertex.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <Rendering/Buffers/VertexBuffer.h>
#include <Rendering/Buffers/IndexBuffer.h>
#include <Rendering/Buffers/InputLayout.h>

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

        //--WIP--
        void LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        // Matrix4F& GetTransform() { return transform; }
        //-------

    private:
        //buffers
        Rendering::Buffers::VertexBuffer m_vertexBuffer;
        Rendering::Buffers::IndexBuffer m_indexBuffer;

        //--WIP--
        Rendering::Buffers::InputLayout m_inputLayout;

        //---SHADER---
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> blob_test;
        //------------

        // Matrix4F transform;
        //-------

        //data
        std::vector<Geometry::Vertex> m_vertices;
        std::vector<unsigned short> m_indices;
    };
}