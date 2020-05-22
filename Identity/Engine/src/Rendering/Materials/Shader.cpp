#include <stdafx.h>

#include <d3dcompiler.h>
#include <Tools/DirectX/GraphicsMacros.h>

#include <Rendering/Materials/Shader.h>

using namespace Engine::Rendering::Materials;

Shader::Shader()
{
    GenConstantBuffers();
}

void Shader::LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path)
{
    HRESULT hr;

    if (m_blob == nullptr)
    {
        int i = 0;
    }

    D3DReadFileToBlob(p_path.c_str(), &m_blob);

    if (m_blob == nullptr)
    {
        int i = 0;
    }

    GFX_THROW_INFO(p_device->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &pixelShader));
}

void Shader::LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &m_blob));
    GFX_THROW_INFO(p_device->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &vertexShader));
}

void Shader::BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->VSSetShader(vertexShader.Get(), nullptr, 0u);
    p_context->PSSetShader(pixelShader.Get(), nullptr, 0u);
}

void Shader::UnbindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->VSSetShader(nullptr, nullptr, 0u);
    p_context->PSSetShader(nullptr, nullptr, 0u);
}

void Shader::BindConstantBuffers(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    m_vcb.Bind();
    m_pcb.Bind();
}

void Shader::UnbindConstantBuffers(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    m_vcb.Unbind();
    m_pcb.Unbind();
}

void Shader::GenConstantBuffers()
{
    m_vcb.GenBuffers();
    m_pcb.GenBuffers();
}
