#include <stdafx.h>
#include <Rendering/Renderer.h>
#include <d3dcompiler.h>
#include <Tools/DirectX/dxerr.h>
#include <Tools/ImGUI/imgui.h>
#include <Tools/ImGUI/imgui_impl_dx11.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectLoader.h>
#include <3DLoader/Manager/ModelManager.h>
#include <Tools/DirectX/GraphicsMacros.h>
#include <Rendering/Light.h>

using namespace Engine::Rendering;

Renderer::Renderer(const HWND p_hwnd)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;     //will change for antialiasing
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = p_hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    HRESULT hr;

    //create the device and swap chain
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
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
    ));

    //get access to back buffer
    Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
    GFX_THROW_INFO(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
    GFX_THROW_INFO(m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_pTarget));


    //Depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthDesc = {};
    depthDesc.DepthEnable = TRUE;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
    GFX_THROW_INFO(m_pDevice->CreateDepthStencilState(&depthDesc, &depthStencilState));

    m_pContext->OMSetDepthStencilState(depthStencilState.Get(), 1u);

    //create depth stencil texture
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = 800u;
    descDepth.Height = 600u;
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;        //will change for antialiasing
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(m_pDevice->CreateTexture2D(&descDepth, nullptr, &depthStencil));

    //create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
    depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthViewDesc.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(m_pDevice->CreateDepthStencilView(depthStencil.Get(), &depthViewDesc, &m_pDepthStencil));

    //bind the depth stencil view
    m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), m_pDepthStencil.Get());

    //add viewport here so we dont create it every frame
    D3D11_VIEWPORT viewPort;
    viewPort.Width = 800;
    viewPort.Height = 600;
    viewPort.MinDepth = 0;
    viewPort.MaxDepth = 1;
    viewPort.TopLeftX = 0;
    viewPort.TopLeftY = 0;
    m_pContext->RSSetViewports(1u, &viewPort);

    ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());
}

Renderer::~Renderer()
{
    ImGui_ImplDX11_Shutdown();
}

void Renderer::EndFrame()
{
    HRESULT hr;

    if (FAILED( hr = m_pSwapChain->Present(1u, 0u)))
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

void Renderer::ClearBuffer(float p_red, float p_green, float p_blue)
{
    const float colour[] = { p_red, p_green, p_blue, 1.0f };
    m_pContext->ClearRenderTargetView(m_pTarget.Get(), colour);
    m_pContext->ClearDepthStencilView(m_pDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void Renderer::DrawObject(std::string p_name, float angle, Vector3F p_pos)
{
    HRESULT hr;

    std::shared_ptr<ObjectElements::Mesh> mesh;
    mesh = Manager::ModelManager::GetInstance()->FindModel(p_name)->GetMeshes()[0];

    // mesh->GenerateBuffers(m_pDevice);
    mesh->Bind(m_pContext);

    // ********* WIP ********* //

    // create constant buffer for transform matrix
    struct VertexConstantBuffer
    {
        Matrix4F model;
        Matrix4F normalModel;
        Matrix4F perspective;
    };

    Vector3D quat{0, 1, 0};
    Matrix4F model = Matrix4F::CreateTransformation(p_pos,
                                                        Quaternion::CreateFromAxisAngle(quat, GPM::Tools::Utils::ToRadians(135.0f)),
                                                        Vector3F{0.02f, 0.02f, 0.02f});

    Matrix4F normalModel = Matrix4F::Inverse(model);
    //Create perspective matrix
    float width = 1.0f;
    float height = 3.0f / 4.0f;
    float NearZ = 0.5f;
    float FarZ = 1000.0f;
    float twoNearZ = NearZ + NearZ;
    float fRange   = FarZ / (FarZ - NearZ);

    Matrix4F perspective{
        twoNearZ / width, 0.0f, 0.0f, 0.0f,
        0.0f, twoNearZ / height, 0.0f, 0.0f,
        0.0f, 0.0f, fRange, 1.0f,
        0.0f, 0.0f, -fRange * NearZ, 0.0f
    };


    model.Transpose();
    normalModel.Transpose();
    perspective.Transpose();

    const VertexConstantBuffer vcb{ model, normalModel,perspective };

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexConstantBuffer;
    D3D11_BUFFER_DESC                    vertexBufferDesc = {};
    vertexBufferDesc.BindFlags                            = D3D11_BIND_CONSTANT_BUFFER;
    vertexBufferDesc.Usage                                = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
    vertexBufferDesc.CPUAccessFlags                       = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags                            = 0u;
    vertexBufferDesc.ByteWidth                            = sizeof(vcb);
    vertexBufferDesc.StructureByteStride                  = 0u;
    D3D11_SUBRESOURCE_DATA VertexConstantShaderData       = {};
    VertexConstantShaderData.pSysMem                      = &vcb;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&vertexBufferDesc, &VertexConstantShaderData, &vertexConstantBuffer));

    //bind the buffer to the shader
    m_pContext->VSSetConstantBuffers(0u, 1u, vertexConstantBuffer.GetAddressOf());

    struct PixelConstantBuffer
    {
        Light lightSource;
        float lightShininess;
    };

    Light dirLight{};
    dirLight.position  = Vector3F(-cos(angle) * 40.0f, 40.0f, -40.0f);
    dirLight.ambient   = Vector3F(0.1f, 0.1f, 0.1f);
    dirLight.diffuse   = Vector3F(1.0f, 1.0f, 0.95f);
    dirLight.specular  = Vector3F(1.0f, 1.0f, 0.95f);
    dirLight.direction = Vector3F(-0.5f, -0.5f, -0.5f).Normalized();
    
    if (ImGui::Begin("Lighting Tool"))
    {
        ImGui::SliderFloat("LightPosX", &dirLight.position.x, -40.0f, 40.0f, "%.1f");
        ImGui::SliderFloat("LightPosY", &dirLight.position.y, -40.0f, 40.0f, "%.1f");
        ImGui::SliderFloat("LightPosZ", &dirLight.position.z, -40.0f, 40.0f, "%.1f");
    }ImGui::End();

    const PixelConstantBuffer pcb{dirLight.position, dirLight.ambient, dirLight.diffuse,
                                  dirLight.specular, dirLight.direction, 32.0f};

    Microsoft::WRL::ComPtr<ID3D11Buffer> pixelConstantBuffer;
    D3D11_BUFFER_DESC                    pixelBufferDesc = {};
    pixelBufferDesc.BindFlags                            = D3D11_BIND_CONSTANT_BUFFER;
    pixelBufferDesc.Usage                                = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
    pixelBufferDesc.CPUAccessFlags                       = D3D11_CPU_ACCESS_WRITE;
    pixelBufferDesc.MiscFlags                            = 0u;
    pixelBufferDesc.ByteWidth                            = sizeof(pcb);
    pixelBufferDesc.StructureByteStride                  = 0u;

    D3D11_SUBRESOURCE_DATA PixelConstantShaderData = {};
    PixelConstantShaderData.pSysMem                = &pcb;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&pixelBufferDesc, &PixelConstantShaderData, &pixelConstantBuffer));

    //bind the buffer to the shader
    m_pContext->PSSetConstantBuffers(0u, 1u, pixelConstantBuffer.GetAddressOf());

    LoadPixelShader(L"../Engine/Resources/Shaders/PixelShader.cso");
    LoadVertexShader(L"../Engine/Resources/Shaders/VertexShader.cso");

    //set primitive draw
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //create input layout
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    const D3D11_INPUT_ELEMENT_DESC            inputDesc[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TxCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20u, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    GFX_THROW_INFO(m_pDevice->CreateInputLayout(inputDesc,
                       std::size(inputDesc),
                       m_blob->GetBufferPointer(),
                       m_blob->GetBufferSize(),
                       &inputLayout));

    m_pContext->IASetInputLayout(inputLayout.Get());

    ImGui_ImplDX11_Init(m_pDevice.Get(), m_pContext.Get());

    GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed(static_cast<UINT>(mesh->m_indices.size()), 0u, 0u));

}

void Renderer::CreateVertexBuffer(Engine::ObjectElements::Mesh* p_mesh) const
{
    HRESULT hr;

    //create vertex buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC vDesc = {};
    vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vDesc.Usage = D3D11_USAGE_DEFAULT;
    vDesc.CPUAccessFlags = 0u;
    vDesc.MiscFlags = 0u;
    vDesc.ByteWidth = sizeof(p_mesh->m_vertices[0]) * p_mesh->m_vertices.size();
    vDesc.StructureByteStride = sizeof(Geometry::Vertex);

    D3D11_SUBRESOURCE_DATA vSD = {};
    vSD.pSysMem = p_mesh->m_vertices.data();
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&vDesc, &vSD, &vertexBuffer));
    const UINT stride = sizeof(Geometry::Vertex);
    const UINT offset = 0u;
    //---------------
    
    //bind vertex buffer
    m_pContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
}

void Renderer::CreateIndexBuffer(Engine::ObjectElements::Mesh* p_mesh) const
{
    HRESULT hr;

    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC inDesc = {};
    inDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    inDesc.Usage = D3D11_USAGE_DEFAULT;
    inDesc.CPUAccessFlags = 0u;
    inDesc.MiscFlags = 0u;
    inDesc.ByteWidth = sizeof(p_mesh->m_indices[0]) * p_mesh->m_indices.size();
    inDesc.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA iSD = {};
    iSD.pSysMem = p_mesh->m_indices.data();
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&inDesc, &iSD, &indexBuffer));
    m_pContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
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

void Renderer::GenerateBuffers()
{
    Manager::ModelManager::GetInstance()->FindModel("statue")->GetMeshes()[0]->GenerateBuffers(m_pDevice);
    // Manager::ModelManager::GetInstance()->FindModel("box")->GetMeshes()[0]->GenerateBuffers(m_pDevice);
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
    return "Identity Renderer Exception";
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
    return "Identity Renderer Info Exception";
}

std::string Renderer::InfoException::GetErrorInfo() const noexcept
{
    return m_info;
}
#pragma endregion

#pragma region DeviceExceptionClass
const char* Renderer::DeviceException::GetType() const noexcept
{
    return "Identity Renderer Exception [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
}
#pragma endregion

#pragma endregion