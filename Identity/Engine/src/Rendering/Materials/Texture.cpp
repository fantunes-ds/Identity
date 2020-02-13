#include <stdafx.h>
#include <Rendering/Materials/Texture.h>
#include "Tools/DirectX/GraphicsMacros.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Engine::Rendering::Materials;

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::LoadTexture(const std::string& p_path)
{
    int width, height, nrChannels;
    stbi_uc* data = stbi_load("../Engine/Resources/rock.jpg", &width, &height, &nrChannels, 0);

    D3D11_TEXTURE2D_DESC textDesc = {};
    textDesc.Width = width;
    textDesc.Height = height;
    textDesc.MipLevels = 1u;
    textDesc.ArraySize = 1u;
    textDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textDesc.SampleDesc.Count = 1;
    textDesc.SampleDesc.Quality = 0;
    textDesc.Usage = D3D11_USAGE_DEFAULT;
    textDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textDesc.CPUAccessFlags = 0;
    textDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subData = {};
    subData.pSysMem = data;
    subData.SysMemPitch = sizeof(data);

    Renderer::GetInstance()->GetDevice()->CreateTexture2D(&textDesc, &subData, m_text.GetAddressOf());
}
