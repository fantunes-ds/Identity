#include <stdafx.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>
#include <Tools/DirectX/GraphicsMacros.h>

using namespace Engine::Rendering::Buffers;

void VertexConstantBuffer::GenBuffers()
{
    HRESULT hr;

    D3D11_BUFFER_DESC                    vertexBufferDesc = {};
    vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0u;
    vertexBufferDesc.ByteWidth = sizeof(m_vcb);
    vertexBufferDesc.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA VertexConstantShaderData = {};
    VertexConstantShaderData.pSysMem = &m_vcb;
    GFX_THROW_INFO(Renderer::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &VertexConstantShaderData, &m_buffer));

    Renderer::GetInstance()->GetContext()->VSSetConstantBuffers(0u, 1u, m_buffer.GetAddressOf());
}

template <typename T>
void VertexConstantBuffer::Update(const T& p_filledBuffer)
{

}
