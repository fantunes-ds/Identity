#include <stdafx.h>
#include <Graphics.h>

Graphics::Graphics(HWND p_hwnd)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = p_hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    //create the device and swap chain
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,    //will change name
        &m_pSwapChain,
        &m_pDevice,
        nullptr,
        &m_pContext
    );

    //get access to back buffer
    ID3D11Resource* backBuffer = nullptr;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
    m_pDevice->CreateRenderTargetView(
        backBuffer,
        nullptr,
        &m_pTarget
    );
    backBuffer->Release();  //we dont need it anymore so lets release it
}

Graphics::~Graphics()
{
    if (m_pTarget != nullptr)
    {
        m_pTarget->Release();
    }

    if (m_pDevice != nullptr)
    {
        m_pDevice->Release();
    }

    if (m_pSwapChain != nullptr)
    {
        m_pSwapChain->Release();
    }

    if (m_pContext != nullptr)
    {
        m_pContext->Release();
    }
}

void Graphics::EndFrame()
{
    m_pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float p_red, float p_green, float p_blue)
{
    const float colour[] = { p_red, p_green, p_blue, 1.0f };
    m_pContext->ClearRenderTargetView(m_pTarget, colour);
}