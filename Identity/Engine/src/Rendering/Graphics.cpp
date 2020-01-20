#include <stdafx.h>
#include <Rendering/Graphics.h>
#include <d3dcompiler.h>

using namespace Engine::Rendering;

Graphics::Graphics(const HWND p_hwnd)
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
        &swapChainDesc,
        &m_pSwapChain,
        &m_pDevice,
        nullptr,
        &m_pContext
    );

    //get access to back buffer
    Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
    m_pDevice->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &m_pTarget
    );
}

void Graphics::EndFrame()
{
    m_pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float p_red, float p_green, float p_blue)
{
    const float colour[] = { p_red, p_green, p_blue, 1.0f };
    m_pContext->ClearRenderTargetView(m_pTarget.Get(), colour);
}

void Graphics::DrawTriangle()
{
    struct Vertex
    {
        float x;
        float y;
    };

    // create vertex buffer (1 2d triangle at center of screen)
    const Vertex vertices[] =
    {
        { 0.0f,0.5f },
        { 0.5f,-0.5f },
        { -0.5f,-0.5f },
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(vertices);
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;
    m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


    //**********MAKE SHADERS************//
    //create vertex shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    D3DReadFileToBlob(L"../Engine/Resources/Shaders/VertexShader.cso", &blob);
    m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
    m_pContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

    //create pixel shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    D3DReadFileToBlob(L"PixelShader.cso", &blob);
    m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
    m_pContext->PSSetShader(pixelShader.Get(), nullptr, 0u);
    //***********END SHADERS************//

    //bind render target
    m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf() , nullptr);

    //set primitive draw
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //create input layout
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    m_pDevice->CreateInputLayout(inputDesc,
        std::size(inputDesc),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &inputLayout);
    m_pContext->IASetInputLayout(inputLayout.Get());


    //configure viewport
    D3D11_VIEWPORT viewPort;
    viewPort.Width = 800;
    viewPort.Height = 600;
    viewPort.MinDepth = 0;
    viewPort.MaxDepth = 1;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    m_pContext->RSSetViewports(1u, &viewPort);


    //m_pContext->IASetVertexBuffers()
    m_pContext->Draw(std::size(vertices), 0u);
}