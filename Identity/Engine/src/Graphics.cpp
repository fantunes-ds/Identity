#include <stdafx.h>

#include <Graphics.h>

Graphics::Graphics(HWND p_hwnd)
{
    DXGI_SWAP_CHAIN_DESC SwapDesc = {};
    SwapDesc.BufferDesc.Width = 0;
    SwapDesc.BufferDesc.Height = 0;
    SwapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapDesc.BufferDesc.RefreshRate.Numerator = 0;
    SwapDesc.BufferDesc.RefreshRate.Denominator = 0;
    SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapDesc.SampleDesc.Count = 1;
    SwapDesc.SampleDesc.Quality = 0;
    SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapDesc.BufferCount = 1;
    SwapDesc.OutputWindow = p_hwnd;
    SwapDesc.Windowed = TRUE;
    SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    SwapDesc.Flags = 0;

    //create the device and swap chain
    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &SwapDesc,    //will change name
        &m_pSwapChain,
        &m_pDevice,
        nullptr,
        &m_pContext
    );
}

Graphics::~Graphics()
{
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
