#include <stdafx.h>
#include <Rendering/Materials/Texture.h>
#include "Tools/DirectX/GraphicsMacros.h"
#include <WICTextureLoader.h>

using namespace Engine::Rendering::Materials;

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(p_device.Get(), p_path.c_str(), m_text.GetAddressOf(), m_texSRV.GetAddressOf()));

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    p_device->CreateSamplerState(&samplerDesc, &m_samplerState);
}

void Texture::BindTexture(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    p_context->PSSetShaderResources(0, 1, m_texSRV.GetAddressOf());
    p_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}
