#include <stdafx.h>

#include <Rendering/Materials/Shader.h>
#include <Tools/DirectX/GraphicsMacros.h>
#include <d3dcompiler.h>

Engine::Rendering::Materials::Shader::Shader()
{
    //GenConstantBuffers();
}

void Engine::Rendering::Materials::Shader::LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
                                                           const std::wstring& p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &blob_test));
    GFX_THROW_INFO(p_device->CreatePixelShader(blob_test->GetBufferPointer(), blob_test->GetBufferSize(), nullptr, &pixelShader));
}

void Engine::Rendering::Materials::Shader::LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
    const std::wstring& p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &blob_test));
    GFX_THROW_INFO(p_device->CreateVertexShader(blob_test->GetBufferPointer(), blob_test->GetBufferSize(), nullptr, &vertexShader));
}

void Engine::Rendering::Materials::Shader::BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const
{
    p_context->PSSetShader(pixelShader.Get(), nullptr, 0u);
    p_context->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

void Engine::Rendering::Materials::Shader::GenConstantBuffers()
{
    m_vcb.GenBuffers();
    m_pcb.GenBuffers();
}
