#include <stdafx.h>
#include <Rendering/Material.h>

void ::Engine::Rendering::Material::AddPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
                                                   const std::wstring& p_path)
{
    m_shader.LoadPixelShader(p_device, p_path);
}

void ::Engine::Rendering::Material::AddVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
    const std::wstring& p_path)
{
    m_shader.LoadVertexShader(p_device, p_path);
}


void Engine::Rendering::Material::AddTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device,
    const std::wstring& p_path)
{
    m_texture.LoadTexture(p_device, p_path);
}

void Engine::Rendering::Material::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    m_shader.BindShader(p_context);
    m_shader.BindConstantBuffers(p_context);
    m_texture.BindTexture(p_context);
}

void Engine::Rendering::Material::Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    m_shader.UnbindShader(p_context);
    m_shader.UnbindConstantBuffers(p_context);
    m_texture.UnbindTexture(p_context);
}
