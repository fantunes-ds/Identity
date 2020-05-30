#include <stdafx.h>

#include <Tools/DirectX/GraphicsMacros.h>

#include <Rendering/Buffers/InputLayout.h>

using namespace Engine::Rendering::Buffers;

void InputLayout::Generate(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const Microsoft::WRL::ComPtr<ID3DBlob>&     p_blob)
{
    HRESULT hr;

    m_inputDesc =
    {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TxCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20u, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    GFX_THROW_INFO(p_device->CreateInputLayout(m_inputDesc.data(), std::size(m_inputDesc),
                   p_blob->GetBufferPointer(), (UINT)p_blob->GetBufferSize(), &m_inputLayout));
}

void InputLayout::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->IASetInputLayout(m_inputLayout.Get());
}

void InputLayout::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->IASetInputLayout(nullptr);
}
