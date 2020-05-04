#include <stdafx.h>

#include <d3dcompiler.h>

#include <Rendering/Materials/VertexShader.h>
#include <Rendering/Renderer.h>

using namespace Engine::Rendering::Materials;

std::shared_ptr<VertexShader> VertexShader::LoadShader(const std::string& p_path, const std::string& p_name)
{
    std::wstring wPath(p_path.begin(), p_path.end());

    std::shared_ptr<VertexShader> tmpVertexShader = std::make_shared<VertexShader>();
    D3DReadFileToBlob(wPath.c_str(), &tmpVertexShader->m_blob);
    if (tmpVertexShader->m_blob == nullptr)
    {
        return nullptr;
    }

    Renderer::GetInstance()->GetDevice()->CreateVertexShader(tmpVertexShader->m_blob->GetBufferPointer(),
                                                             tmpVertexShader->m_blob->GetBufferSize(),
                                                             nullptr,
                                                             &tmpVertexShader->vertexShader);

    return tmpVertexShader;
}

void VertexShader::BindShader() const
{
    Renderer::GetInstance()->GetContext()->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

void VertexShader::UnBindShader()
{
    Renderer::GetInstance()->GetContext()->VSSetShader(nullptr, nullptr, 0u);
}

void VertexShader::GenerateConstantBuffer()
{
    m_vcb.GenBuffers();
}

void VertexShader::BindConstantBuffer() const
{
    m_vcb.Bind();
}

void VertexShader::UnBindConstantBuffer() const
{
    m_vcb.Unbind();
}
