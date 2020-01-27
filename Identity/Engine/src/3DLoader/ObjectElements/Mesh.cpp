#include <stdafx.h>

#include <3DLoader/ObjectElements/Mesh.h>
#include <Tools/DirectX/GraphicsMacros.h>

Engine::ObjectElements::Mesh::Mesh(std::vector<Engine::Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices) :
    m_vertices { p_vertices }, m_indices { p_indices } {}

Engine::ObjectElements::Mesh::Mesh(const Mesh& p_other):
    m_vertices { p_other.m_vertices }, m_indices { p_other.m_indices } {}

void Engine::ObjectElements::Mesh::GenerateBuffers(Microsoft::WRL::ComPtr<ID3D11Device> p_device)
{
    m_vertexBuffer.Generate(p_device, m_vertices);
    m_indexBuffer.Generate(p_device, m_indices);
}

void Engine::ObjectElements::Mesh::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context)
{
    m_vertexBuffer.Bind(p_context);
    m_indexBuffer.Bind(p_context);
}
