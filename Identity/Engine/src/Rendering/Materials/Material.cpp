#include <stdafx.h>

#include <d3dcompiler.inl>

#include <Rendering/Materials/Material.h>

using namespace Engine::Rendering::Materials;

Material::Material()
{
    m_pixelShader  = std::make_shared<PixelShader>();
    m_vertexShader = std::make_shared<VertexShader>();
    m_texture      = std::make_shared<Texture>();
}

std::shared_ptr<Material> Material::CreateMaterial(const std::shared_ptr<PixelShader> p_pixelShader, const std::shared_ptr<VertexShader> p_vertexShader)
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

void Material::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
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

void Material::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
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

const Microsoft::WRL::ComPtr<ID3DBlob> Material::GetBlob()
{
    return m_vertexShader->GetBlob();
}

void Material::Serialize(std::ostream& p_stream)
{
    if (m_textureState)
        p_stream << "MATERIAL " << GetName() << " "
        << GetPixelShader()->GetName() << " "
        << GetVertexShader()->GetName() << " "
        << GetTexture()->GetName() << " "
        << std::to_string(GetColor().x) << " " << std::to_string(GetColor().y) << " " << std::to_string(GetColor().z) << '\n';
    else
        p_stream << "MATERIAL " << GetName() << " "
        << GetPixelShader()->GetName() << " "
        << GetVertexShader()->GetName() << " "
        << "NO_TEXTURE" << " "
        << GetColor().x << " " << GetColor().y << " " << GetColor().z << '\n';
}
