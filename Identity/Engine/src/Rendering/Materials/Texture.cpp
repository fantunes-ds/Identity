#include <stdafx.h>
#include <Rendering/Materials/Texture.h>
#include "Tools/DirectX/GraphicsMacros.h"

#include <WICTextureLoader.h>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

using namespace Engine::Rendering::Materials;

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::LoadTexture(const wchar_t* p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(DirectX::CreateWICTextureFromFile(Renderer::GetInstance()->GetDevice().Get(), p_path, m_text.GetAddressOf(), m_texture.GetAddressOf()));

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    Renderer::GetInstance()->GetDevice()->CreateSamplerState(&samplerDesc, &m_samplerState);
}
