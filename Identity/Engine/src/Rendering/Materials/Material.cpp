#include <stdafx.h>
#include <Rendering/Materials/Material.h>
#include <d3dcompiler.inl>

Engine::Rendering::Materials::Material::Material()
{
    m_pixelShader = std::make_shared<PixelShader>();
    m_vertexShader = std::make_shared < VertexShader>();
    m_texture = std::make_shared<Texture>();
}

std::shared_ptr<Engine::Rendering::Materials::Material> Engine::Rendering::Materials::Material::CreateMaterial(
    const std::shared_ptr<PixelShader> p_pixelShader, const std::shared_ptr<VertexShader> p_vertexShader)
{
    if (p_pixelShader == nullptr || p_vertexShader == nullptr)
    {
        return nullptr;
    }

    std::shared_ptr<Material> tmpMaterial = std::make_shared<Material>();
    tmpMaterial->SetPixelShader(p_pixelShader);
    tmpMaterial->SetVertexShader(p_vertexShader);

    return tmpMaterial;
}

void Engine::Rendering::Materials::Material::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    if (m_pixelShader)
    {
        m_pixelShader->BindShader();
        m_pixelShader->BindConstantBuffer();
    }
    if (m_vertexShader)
    {
        m_vertexShader->BindShader();
        m_vertexShader->BindConstantBuffer();
    }

    if (m_texture)
        m_texture->BindTexture();
}

void Engine::Rendering::Materials::Material::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    if (m_pixelShader)
    {
        m_pixelShader->UnBindShader();
        m_pixelShader->UnBindConstantBuffer();
    }
    if (m_vertexShader)
    {
        m_vertexShader->UnBindShader();
        m_vertexShader->UnBindConstantBuffer();
    }

    if (m_texture)
        m_texture->UnbindTexture();
}

const Microsoft::WRL::ComPtr<ID3DBlob> Engine::Rendering::Materials::Material::GetBlob()
{
    return m_vertexShader->GetBlob();
}
