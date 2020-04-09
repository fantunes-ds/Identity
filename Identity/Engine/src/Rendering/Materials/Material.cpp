#include <stdafx.h>
#include <Rendering/Materials/Material.h>


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

void Engine::Rendering::Materials::Material::AddPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
                                                            const std::wstring& p_path)
{
    m_shader.LoadPixelShader(p_device, p_path);
}

void Engine::Rendering::Materials::Material::AddVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
    const std::wstring& p_path)
{
    m_shader.LoadVertexShader(p_device, p_path);
}


void Engine::Rendering::Materials::Material::AddTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
    const std::wstring& p_path)
{
    m_texture.LoadTexture(p_device, p_path);
}

void Engine::Rendering::Materials::Material::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    m_shader.BindShader(p_context);
    m_shader.BindConstantBuffers(p_context);
    m_texture.BindTexture(p_context);
}

void Engine::Rendering::Materials::Material::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    m_shader.UnbindShader(p_context);
    m_shader.UnbindConstantBuffers(p_context);
    m_texture.UnbindTexture(p_context);
}