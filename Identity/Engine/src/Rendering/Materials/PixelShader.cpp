#include <stdafx.h>

#include <d3dcompiler.h>

#include <Rendering/Materials/PixelShader.h>
#include <Rendering/Renderer.h>

using namespace Engine::Rendering::Materials;

std::shared_ptr<PixelShader> PixelShader::LoadShader(const std::string& p_path, const std::string& p_name)
{
    std::wstring wPath(p_path.begin(), p_path.end());

    std::shared_ptr<PixelShader> tmpPixelShader = std::make_shared<PixelShader>();
    D3DReadFileToBlob(wPath.c_str(), &tmpPixelShader->m_blob);
    if (tmpPixelShader->m_blob == nullptr)
    {
        return nullptr;
    }

    Renderer::GetInstance()->GetDevice()->CreatePixelShader(tmpPixelShader->m_blob->GetBufferPointer(), tmpPixelShader->m_blob->GetBufferSize(),
                                                            nullptr, &tmpPixelShader->pixelShader);

    return tmpPixelShader;
}

void PixelShader::BindShader() const
{
    Renderer::GetInstance()->GetContext()->PSSetShader(pixelShader.Get(), nullptr, 0u);
}

void PixelShader::UnBindShader()
{
    Renderer::GetInstance()->GetContext()->PSSetShader(nullptr, nullptr, 0u);
}

void PixelShader::GenerateConstantBuffer()
{
    m_pcb.GenBuffers();
}

void PixelShader::BindConstantBuffer() const
{
    m_pcb.Bind();
}

void PixelShader::UnBindConstantBuffer() const
{
    m_pcb.Unbind();
}
