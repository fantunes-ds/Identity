#include <stdafx.h>

#include <3DLoader/ObjectElements/Mesh.h>
#include <Tools/DirectX/GraphicsMacros.h>
#include <Systems/TransformSystem.h>

Engine::ObjectElements::Mesh::Mesh(std::vector<Engine::Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices) :
    m_vertices { p_vertices }, m_indices { p_indices }
{
    m_transform = Containers::TransformSystem::AddTransform();
}

Engine::ObjectElements::Mesh::Mesh(const Mesh& p_other):
    m_vertices{ p_other.m_vertices }, m_indices{ p_other.m_indices }, m_transform{ p_other.m_transform } {}

void Engine::ObjectElements::Mesh::GenerateBuffers(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device)
{
    if (m_material < 0)
        SetMaterial(Containers::MaterialContainer::FindMaterial("missing"));

    m_vertexBuffer.Generate(p_device, m_vertices);
    m_indexBuffer.Generate(p_device, m_indices);
    m_inputLayout.Generate(p_device, GetMaterial().GetShader().GetBlob());
}

void Engine::ObjectElements::Mesh::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    GetMaterial().Bind(p_context);
    m_vertexBuffer.Bind(p_context);
    m_indexBuffer.Bind(p_context);
    m_inputLayout.Bind(p_context);
}

void Engine::ObjectElements::Mesh::SetMaterial(const int32_t p_material)
{
    m_material = p_material;
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
