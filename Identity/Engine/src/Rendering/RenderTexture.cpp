#include <stdafx.h>
#include <Rendering/RenderTexture.h>
#include <Rendering/Renderer.h>
#include <Tools/DirectX/GraphicsMacros.h>

#include <utility>

Engine::Rendering::RenderTexture::RenderTexture(UINT p_width, UINT p_height, bool p_createDepthBuffer) : m_width(p_width), m_height(p_height)
{
    CreateTexture(p_width, p_height);
    if (p_createDepthBuffer)
        CreateDepthStencilBuffer(p_width, p_height);
}

Engine::Rendering::RenderTexture::RenderTexture(UINT p_width, UINT p_height, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> p_stencil)
{
    CreateTexture(p_width, p_height);
    m_stencilView = std::move(p_stencil);
}

void Engine::Rendering::RenderTexture::CreateTexture(UINT p_width, UINT p_height)
{
    HRESULT hr;

    D3D11_TEXTURE2D_DESC            textureDesc{};
    D3D11_RENDER_TARGET_VIEW_DESC   renderTargetViewDesc{};
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};


    // Setup the texture description.
    // We will have our map be a square
    // We will need to have this texture bound as a render target AND a shader resource
    textureDesc.Width = p_width;
    textureDesc.Height = p_height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    // Create the texture
    GFX_THROW_INFO(Rendering::Renderer::GetInstance()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture));

    ///////////////////////// Map's Render Target
    //// Setup the description of the render target view.
    renderTargetViewDesc.Format = textureDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // Create the render target view.
    GFX_THROW_INFO(Rendering::Renderer::GetInstance()->GetDevice()->CreateRenderTargetView(*m_renderTargetTexture.GetAddressOf(), &renderTargetViewDesc, &m_renderTargetView));
    /////////////////////// Map's Shader Resource View
    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels = 1;

    //// Create the shader resource view.
    GFX_THROW_INFO(Rendering::Renderer::GetInstance()->GetDevice()->CreateShaderResourceView(*m_renderTargetTexture.GetAddressOf(), &shaderResourceViewDesc, &m_renderShaderResourceView));
}

void Engine::Rendering::RenderTexture::CreateDepthStencilBuffer(UINT p_width, UINT p_height)
{
    HRESULT hr;

    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
    GFX_THROW_INFO(Rendering::Renderer::GetInstance()->GetDevice()->CreateDepthStencilState(&dsDesc, &pDSState));

    Rendering::Renderer::GetInstance()->GetContext()->OMSetDepthStencilState(pDSState.Get(), 1u);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = p_width;
    descDepth.Height = p_height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;

    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    GFX_THROW_INFO(Rendering::Renderer::GetInstance()->GetDevice()->CreateTexture2D(&descDepth, NULL, &pDepthStencil));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(Rendering::Renderer::GetInstance()->GetDevice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_stencilView));

    Renderer::GetInstance()->GetContext()->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), *m_stencilView.GetAddressOf());
}

const void Engine::Rendering::RenderTexture::Bind() const
{
    if (*m_stencilView.GetAddressOf() != nullptr)
        Renderer::GetInstance()->GetContext()->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), *m_stencilView.GetAddressOf());
    else
        Renderer::GetInstance()->GetContext()->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
}
