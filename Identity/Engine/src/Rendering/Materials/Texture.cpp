#include <stdafx.h>

#include <WICTextureLoader.h>

#include <Rendering/Materials/Texture.h>
#include <Rendering/Renderer.h>

using namespace Engine::Rendering::Materials;

Texture::Texture()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter             = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU           = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV           = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW           = D3D11_TEXTURE_ADDRESS_WRAP;

    Renderer::GetInstance()->GetDevice()->CreateSamplerState(&samplerDesc, &m_samplerState);
}

std::shared_ptr<Texture> Texture::LoadTexture(const std::string& p_path, const std::string& p_name)
{
    std::wstring wPath(p_path.begin(), p_path.end());

    std::shared_ptr<Texture> tmpTexture = std::make_shared<Texture>();

    DirectX::CreateWICTextureFromFile(Renderer::GetInstance()->GetDevice().Get(), wPath.c_str(),
                                      tmpTexture->m_texture.GetAddressOf(), tmpTexture->m_textureShaderResourceView.GetAddressOf());

    if (tmpTexture->m_texture == nullptr || tmpTexture->m_textureShaderResourceView == nullptr)
    {
        return nullptr;
    }

    return tmpTexture;
}

void Texture::BindTexture()
{
    Renderer::GetInstance()->GetContext()->PSSetShaderResources(0, 1, m_textureShaderResourceView.GetAddressOf());
    Renderer::GetInstance()->GetContext()->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
}

void Texture::UnbindTexture()
{
    Renderer::GetInstance()->GetContext()->PSSetShaderResources(0, 0, nullptr);
    Renderer::GetInstance()->GetContext()->PSSetSamplers(0, 0, nullptr);
}
