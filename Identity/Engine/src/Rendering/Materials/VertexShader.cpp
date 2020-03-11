#include <stdafx.h>
#include <Rendering/Materials/VertexShader.h>
#include <Rendering/Renderer.h>
#include <d3dcompiler.h>

Engine::Rendering::Materials::VertexShader::VertexShader()
{
    //TODO
}

Engine::Rendering::Materials::VertexShader::~VertexShader()
{
    //TODO
}

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
        &tmpVertexShader->pixelShader);

    return tmpVertexShader;
}

void Engine::Rendering::Materials::VertexShader::LoadShader(const std::string& p_path)
{
    //TODO
}

void Engine::Rendering::Materials::VertexShader::BindShader()
{
    //TODO
}

void Engine::Rendering::Materials::VertexShader::GenerateConstantBuffer()
{
    //TODO
}
