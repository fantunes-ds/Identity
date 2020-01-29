#include <stdafx.h>
#include <Rendering/Buffers/VertexBuffer.h>
#include "Tools/DirectX/GraphicsMacros.h"

void Engine::Rendering::Buffers::VertexBuffer::Generate(Microsoft::WRL::ComPtr<ID3D11Device> p_device,
                                                        std::vector<Geometry::Vertex> p_vertices)
{
    HRESULT hr;

    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.CPUAccessFlags = 0u;
    descriptor.MiscFlags = 0u;
    descriptor.ByteWidth = sizeof(p_vertices[0]) * p_vertices.size();
    descriptor.StructureByteStride = sizeof(Geometry::Vertex);

    subData.pSysMem = p_vertices.data();
    GFX_THROW_INFO(p_device->CreateBuffer(&descriptor, &subData, &buffer));
    p_device->CreateBuffer(&descriptor, &subData, &buffer);
    stride = sizeof(Geometry::Vertex);
    offset = 0u;
}

void Engine::Rendering::Buffers::VertexBuffer::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context)
{
    p_context->IASetVertexBuffers(0u, 1u, buffer.GetAddressOf(), &stride, &offset);
}
