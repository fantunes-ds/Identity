#include <stdafx.h>
#include <Rendering/Buffers/PixelConstantBuffer.h>
#include <Rendering/Renderer.h>
#include <Tools/DirectX/GraphicsMacros.h>

using namespace Engine::Rendering::Buffers;

void PixelConstantBuffer::GenBuffers()
{
    HRESULT hr;

    D3D11_BUFFER_DESC                    pixelBufferDesc = {};
    pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
    pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    pixelBufferDesc.MiscFlags = 0u;
    pixelBufferDesc.ByteWidth = sizeof(m_pcb);
    pixelBufferDesc.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA PixelConstantShaderData = {};
    PixelConstantShaderData.pSysMem = &m_pcb;
    GFX_THROW_INFO(Renderer::GetInstance()->GetDevice()->CreateBuffer(&pixelBufferDesc, &PixelConstantShaderData, &m_buffer));

    Renderer::GetInstance()->GetContext()->PSSetConstantBuffers(0u, 1u, m_buffer.GetAddressOf());
}

template <typename T>
void PixelConstantBuffer::Update(const T& p_filledBuffer)
{

}