#include <stdafx.h>

#include <3DLoader/ObjectElements/Mesh.h>
#include <Tools/DirectX/GraphicsMacros.h>
#include <Systems/TransformSystem.h>
#include "Managers/ResourceManager.h"

Engine::ObjectElements::Mesh::Mesh(std::vector<Engine::Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices) :
    m_vertices { p_vertices }, m_indices { p_indices }
{
    m_transform = Containers::TransformSystem::AddTransform();
}

Engine::ObjectElements::Mesh::Mesh(const Mesh& p_other):
    m_vertices{ p_other.m_vertices }, m_indices{ p_other.m_indices }, m_transform{ p_other.m_transform } {}

void Engine::ObjectElements::Mesh::GenerateBuffers(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device)
{
    // if (m_material < 0)
        // SetMaterial(Containers::MaterialContainer::FindMaterial("missing"));
    if (m_material == nullptr)
        SetMaterial(Managers::ResourceManager::GetMaterial("default"));

    m_vertexBuffer.Generate(p_device, m_vertices);
    m_indexBuffer.Generate(p_device, m_indices);
    m_inputLayout.Generate(p_device, GetMaterial()->GetBlob());
}

void Engine::ObjectElements::Mesh::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    GetMaterial()->Bind(p_context);
    m_vertexBuffer.Bind(p_context);
    m_indexBuffer.Bind(p_context);
    m_inputLayout.Bind(p_context);
}

void Engine::ObjectElements::Mesh::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    m_inputLayout.Unbind(p_context);
    m_indexBuffer.Unbind(p_context);
    m_vertexBuffer.Unbind(p_context);
    GetMaterial()->Unbind(p_context);
}

void Engine::ObjectElements::Mesh::SetMaterial(std::shared_ptr<Rendering::Materials::Material> p_material)
{
    if (p_material)
        m_material = p_material;
    else
        m_material = Managers::ResourceManager::GetMaterial("default");
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
