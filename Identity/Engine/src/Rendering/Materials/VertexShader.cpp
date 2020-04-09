#include <stdafx.h>
#include <Rendering/Materials/VertexShader.h>
#include <Rendering/Renderer.h>
#include <d3dcompiler.h>

std::shared_ptr<Engine::Rendering::Materials::VertexShader> Engine::Rendering::Materials::VertexShader::LoadShader(
    const std::string& p_path, const std::string& p_name)
{
    std::wstring wPath(p_path.begin(), p_path.end());

    std::shared_ptr<VertexShader> tmpVertexShader = std::make_shared<VertexShader>();
    D3DReadFileToBlob(wPath.c_str(), &tmpVertexShader->m_blob);
    if (tmpVertexShader->m_blob == nullptr)
    {
        return nullptr;
    }

    Rendering::Renderer::GetInstance()->GetDevice()->CreateVertexShader(
        tmpVertexShader->m_blob->GetBufferPointer(),
        tmpVertexShader->m_blob->GetBufferSize(),
        nullptr,
        &tmpVertexShader->vertexShader);

    return tmpVertexShader;
}

void Engine::Rendering::Materials::VertexShader::BindShader() const
{
    Rendering::Renderer::GetInstance()->GetContext()->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

void Engine::Rendering::Materials::VertexShader::UnBindShader()
{
    Rendering::Renderer::GetInstance()->GetContext()->VSSetShader(nullptr, nullptr, 0u);
}

void Engine::Rendering::Materials::VertexShader::GenerateConstantBuffer()
{
    m_vcb.GenBuffers();
}

void Engine::Rendering::Materials::VertexShader::BindConstantBuffer() const
{
    m_vcb.Bind();
}

void Engine::Rendering::Materials::VertexShader::UnBindConstantBuffer() const
{
    m_vcb.Unbind();
}
