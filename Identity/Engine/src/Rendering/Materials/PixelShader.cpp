#include <stdafx.h>
#include <Rendering/Materials/PixelShader.h>
#include <Rendering/Renderer.h>
#include <d3dcompiler.h>

std::shared_ptr<Engine::Rendering::Materials::PixelShader> Engine::Rendering::Materials::PixelShader::LoadShader(
    const std::string& p_path, const std::string& p_name)
{
    std::wstring wPath(p_path.begin(), p_path.end());

    std::shared_ptr<PixelShader> tmpPixelShader = std::make_shared<PixelShader>();
    D3DReadFileToBlob(wPath.c_str(), &tmpPixelShader->m_blob);
    if (tmpPixelShader->m_blob == nullptr)
    {
        return nullptr;
    }

    Rendering::Renderer::GetInstance()->GetDevice()->CreatePixelShader(
                                                                       tmpPixelShader->m_blob->GetBufferPointer(),
                                                                       tmpPixelShader->m_blob->GetBufferSize(),
                                                                       nullptr,
                                                                       &tmpPixelShader->pixelShader);

    return tmpPixelShader;
}

void Engine::Rendering::Materials::PixelShader::BindShader() const
{
    Rendering::Renderer::GetInstance()->GetContext()->PSSetShader(pixelShader.Get(), nullptr, 0u);
}

void Engine::Rendering::Materials::PixelShader::UnBindShader()
{
    Rendering::Renderer::GetInstance()->GetContext()->PSSetShader(nullptr, nullptr, 0u);
}

void Engine::Rendering::Materials::PixelShader::GenerateConstantBuffer()
{
    m_pcb.GenBuffers();
}

void Engine::Rendering::Materials::PixelShader::BindConstantBuffer() const
{
    m_pcb.Bind();
}

void Engine::Rendering::Materials::PixelShader::UnBindConstantBuffer() const
{
    m_pcb.Unbind();
}
