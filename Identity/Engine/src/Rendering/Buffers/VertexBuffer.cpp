#include <stdafx.h>

#include <Tools/DirectX/GraphicsMacros.h>

#include <Rendering/Buffers/VertexBuffer.h>

void Engine::Rendering::Buffers::VertexBuffer::Generate(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
                            std::vector<Geometry::Vertex>&              p_vertices)
{
    HRESULT hr;

    m_descriptor.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
    m_descriptor.Usage               = D3D11_USAGE_DEFAULT;
    m_descriptor.CPUAccessFlags      = 0u;
    m_descriptor.MiscFlags           = 0u;
    m_descriptor.ByteWidth           = sizeof(p_vertices[0]) * p_vertices.size();
    m_descriptor.StructureByteStride = sizeof(Geometry::Vertex);

    m_subData.pSysMem = p_vertices.data();
    GFX_THROW_INFO(p_device->CreateBuffer(&m_descriptor, &m_subData, &m_buffer));
    p_device->CreateBuffer(&m_descriptor, &m_subData, &m_buffer);
    m_stride = sizeof(Geometry::Vertex);
    m_offset = 0u;
}

void Engine::Rendering::Buffers::VertexBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    p_context->IASetVertexBuffers(0u, 1u, m_buffer.GetAddressOf(), &m_stride, &m_offset);
}

void Engine::Rendering::Buffers::VertexBuffer::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    p_context->IASetVertexBuffers(0u, 0, nullptr, nullptr, nullptr);
}

bool Engine::Rendering::Buffers::VertexBuffer::operator==(const Engine::Rendering::Buffers::VertexBuffer& p_other) const
{
    if (m_stride == p_other.m_stride && m_offset == p_other.m_offset)
        return true;

    return false;
}
