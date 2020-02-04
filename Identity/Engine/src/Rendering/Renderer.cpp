#include <stdafx.h>
#include <Rendering/Renderer.h>
#include <d3dcompiler.h>
#include <Tools/DirectX/dxerr.h>
#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <3DLoader/ObjectLoader.h>
#include <Tools/DirectX/GraphicsMacros.h>

using namespace Engine::Rendering;

Renderer::Renderer(const HWND p_hwnd, const int p_clientWidth, const int p_clientHeight) : m_width(p_clientWidth), m_height(p_clientHeight)
{
    HRESULT hr;

    //create the device
    GFX_THROW_INFO(
    D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &m_pDevice,
        nullptr,
        &m_pContext
    ));

    //check for msaa quality support
    UINT m4xMsaaQuality;
    GFX_THROW_INFO(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));

    //describe the swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //with 4x msaa
    if (m_enable4xMSAA)
    {
        swapChainDesc.SampleDesc.Count = 4;
        swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    //without
    else
    {
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
    }

    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = p_hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    //Get the factory to generate the swapChain
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    GFX_THROW_INFO(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), &dxgiDevice));

    Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
    GFX_THROW_INFO(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), &dxgiAdapter));

    Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
    GFX_THROW_INFO(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory));

    GFX_THROW_INFO(dxgiFactory->CreateSwapChain(m_pDevice.Get(), &swapChainDesc, &m_pSwapChain));

    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();


    //create render target view
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    GFX_THROW_INFO(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
    GFX_THROW_INFO(m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_pTarget));
    

    D3D11_TEXTURE2D_DESC depthStencilDesc;
    depthStencilDesc.Width = m_width;
    depthStencilDesc.Height = m_height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    //with 4x msaa
    if (m_enable4xMSAA)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
    }
    //without
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    GFX_THROW_INFO(m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer));
    GFX_THROW_INFO(m_pDevice->CreateDepthStencilView(depthStencilBuffer.Get(), 0, &m_pDepthStencil));

    m_pContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), m_pDepthStencil.Get());

    D3D11_VIEWPORT viewPort;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.Width = static_cast<float>(m_width);
    viewPort.Height = static_cast<float>(m_height);
    viewPort.MinDepth = 0;
    viewPort.MaxDepth = 1;
    m_pContext->RSSetViewports(1u, &viewPort);

    ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());
}

Renderer::~Renderer()
{
    ImGui_ImplDX11_Shutdown();
}

void Renderer::EndFrame() const
{
    HRESULT hr;

    if (FAILED(hr = m_pSwapChain->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(m_pDevice->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT(hr);
        }
    }
}

void Renderer::ClearBuffer(float p_red, float p_green, float p_blue) const
{
    const float colour[] = { p_red, p_green, p_blue, 1.0f };
    m_pContext->ClearRenderTargetView(m_pTarget.Get(), colour);
    m_pContext->ClearDepthStencilView(m_pDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void Renderer::LoadPixelShader(const std::wstring& p_path)
{
    HRESULT hr;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &m_blob));
    GFX_THROW_INFO(m_pDevice->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &pixelShader));
    m_pContext->PSSetShader(pixelShader.Get(), nullptr, 0u);
}

void Renderer::LoadVertexShader(const std::wstring& p_path)
{
    HRESULT hr;

    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &m_blob));
    GFX_THROW_INFO(m_pDevice->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &vertexShader));
    m_pContext->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

void Renderer::Resize(const int p_width, const int p_height)
{
    m_width = p_width;
    m_height = p_height;

    DXGI_MODE_DESC modeDesc = {};
    modeDesc.Width = 0;
    modeDesc.Height = 0;
    modeDesc.RefreshRate.Numerator = 0;
    modeDesc.RefreshRate.Denominator = 0;
    modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    m_pSwapChain->ResizeTarget(&modeDesc);

    D3D11_VIEWPORT viewPort;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    viewPort.Width = static_cast<float>(m_width);
    viewPort.Height = static_cast<float>(m_height);
    viewPort.MinDepth = 0;
    viewPort.MaxDepth = 1;
    m_pContext->RSSetViewports(1u, &viewPort);
}

void Renderer::GetResolution(int& p_width, int& p_height)
{
    p_width = m_width;
    p_height = m_height;
}

#pragma region ExceptionsClass

#pragma region HrExceptionClass
Renderer::HrException::HrException(int p_line, const char* p_file, HRESULT p_hr,
    std::vector<std::string> p_infoMsg) noexcept
    : Exception(p_line, p_file),
    m_hr(p_hr)
{
    for (const auto& msg : p_infoMsg)
    {
        m_info += msg;
        m_info.push_back('\n');
    }
    if (!m_info.empty())
    {
        m_info.pop_back();
    }
}

const char* Renderer::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[ERROR CODE] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << GetErrorCode() << ")" << std::endl
        << "[ERROR STRING] " << GetErrorString() << std::endl
        << "[DESCRIPTION] " << GetErrorDescription() << std::endl;

    if (!m_info.empty())
    {
        oss << "\n[ERROR INFO]\n" << GetErrorInfo() << std::endl;
    }

    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Renderer::HrException::GetType() const noexcept
{
    return "Identity Graphics Exception";
}

HRESULT Renderer::HrException::GetErrorCode() const noexcept
{
    return m_hr;
}

std::string Renderer::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(m_hr);
}

std::string Renderer::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(m_hr, buf, sizeof(buf));
    return buf;
}

std::string Renderer::HrException::GetErrorInfo() const noexcept
{
    return m_info;
}
#pragma endregion

#pragma region InfoExceptionClass
Renderer::InfoException::InfoException(int p_line, const char* p_file, std::vector<std::string> p_infoMsg) noexcept
    : Exception(p_line, p_file)
{
    for (const auto& msg : p_infoMsg)
    {
        m_info += msg;
        m_info.push_back('\n');
    }
    if (!m_info.empty())
    {
        m_info.pop_back();
    }
}

const char* Renderer::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[ERROR INFO]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Renderer::InfoException::GetType() const noexcept
{
    return "Identity Graphics Info Exception";
}

std::string Renderer::InfoException::GetErrorInfo() const noexcept
{
    return m_info;
}
#pragma endregion

#pragma region DeviceExceptionClass
const char* Renderer::DeviceException::GetType() const noexcept
{
    return "Identity Graphics Exception [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
}
#pragma endregion

#pragma endregion