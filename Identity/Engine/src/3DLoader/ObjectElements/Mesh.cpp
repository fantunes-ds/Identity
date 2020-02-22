#include <stdafx.h>

#include <3DLoader/ObjectElements/Mesh.h>
#include <Tools/DirectX/GraphicsMacros.h>

Engine::ObjectElements::Mesh::Mesh(std::vector<Engine::Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices) :
    m_vertices { p_vertices }, m_indices { p_indices } {}

Engine::ObjectElements::Mesh::Mesh(const Mesh& p_other):
    m_vertices { p_other.m_vertices }, m_indices { p_other.m_indices } {}

void Engine::ObjectElements::Mesh::GenerateBuffers(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device)
{
    m_material.AddPixelShader(p_device, L"../Engine/Resources/Shaders/PixelShader.cso");
    m_material.AddVertexShader(p_device, L"../Engine/Resources/Shaders/VertexShader.cso");
    m_vertexBuffer.Generate(p_device, m_vertices);
    m_indexBuffer.Generate(p_device, m_indices);
    m_inputLayout.Generate(p_device, m_material.GetShader().GetBlob());
}

void Engine::ObjectElements::Mesh::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    m_material.Bind(p_context);
    m_vertexBuffer.Bind(p_context);
    m_indexBuffer.Bind(p_context);
    m_inputLayout.Bind(p_context);
}

bool Engine::ObjectElements::Mesh::operator==(const Mesh& p_other) const
{
    if (m_vertices.size() != p_other.m_vertices.size() || m_indices != p_other.m_indices)
        return false;

    if (m_vertexBuffer == p_other.m_vertexBuffer)
        return true;

    return false;
}

bool Engine::ObjectElements::Mesh::operator!=(const Mesh& p_other) const
{
    if (m_vertices.size() != p_other.m_vertices.size() || m_indices != p_other.m_indices)
        return true;

    if (m_vertexBuffer == p_other.m_vertexBuffer)
        return false;

    return true;
}
