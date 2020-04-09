#include <stdafx.h>
#include <Rendering/Buffers/IndexBuffer.h>
#include "Tools/DirectX/GraphicsMacros.h"

void Engine::Rendering::Buffers::IndexBuffer::Generate(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, std::vector<unsigned short> p_indices)
{
    HRESULT hr;

    m_descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_descriptor.Usage = D3D11_USAGE_DEFAULT;
    m_descriptor.CPUAccessFlags = 0u;
    m_descriptor.MiscFlags = 0u;
    m_descriptor.ByteWidth = sizeof(p_indices[0]) * p_indices.size();
    m_descriptor.StructureByteStride = sizeof(unsigned short);

    m_subData.pSysMem = p_indices.data();
    GFX_THROW_INFO(p_device->CreateBuffer(&m_descriptor, &m_subData, &m_buffer));
}

void Engine::Rendering::Buffers::IndexBuffer::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

void Engine::Rendering::Buffers::IndexBuffer::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0u);
}
