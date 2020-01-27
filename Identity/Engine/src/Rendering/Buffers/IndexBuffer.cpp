#include <stdafx.h>
#include <Rendering/Buffers/IndexBuffer.h>
#include "Tools/DirectX/GraphicsMacros.h"

void Engine::Rendering::Buffers::IndexBuffer::Generate(Microsoft::WRL::ComPtr<ID3D11Device> p_device, std::vector<unsigned short> p_indices)
{
    HRESULT hr;

    descriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.CPUAccessFlags = 0u;
    descriptor.MiscFlags = 0u;
    descriptor.ByteWidth = sizeof(p_indices[0]) * p_indices.size();
    descriptor.StructureByteStride = sizeof(unsigned short);

    subData.pSysMem = p_indices.data();
    GFX_THROW_INFO(p_device->CreateBuffer(&descriptor, &subData, &buffer));
}

void Engine::Rendering::Buffers::IndexBuffer::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context)
{
    p_context->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
