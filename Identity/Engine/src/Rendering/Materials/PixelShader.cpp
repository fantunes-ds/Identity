#include <stdafx.h>
#include <Rendering/Materials/PixelShader.h>
#include <Rendering/Renderer.h>
#include <d3dcompiler.h>

Engine::Rendering::Materials::PixelShader::PixelShader()
{
    //TODO
}

Engine::Rendering::Materials::PixelShader::~PixelShader()
{
    //TODO
}

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

void Engine::Rendering::Materials::PixelShader::LoadShader(const std::string& p_path)
{
    //TODO
}

void Engine::Rendering::Materials::PixelShader::BindShader()
{
    //TODO
}

void Engine::Rendering::Materials::PixelShader::GenerateConstantBuffer()
{
    //TODO
}
