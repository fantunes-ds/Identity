#include <stdafx.h>
#include <Rendering/Graphics.h>
#include <d3dcompiler.h>
#include <Tools/dxerr.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectLoader.h>

using namespace Engine::Rendering;

#pragma region ExcpetionsMacro

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr)

#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)

#pragma endregion

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
}

void Graphics::EndFrame()
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

void Graphics::ClearBuffer(float p_red, float p_green, float p_blue)
{
    const float colour[] = { p_red, p_green, p_blue, 1.0f };
    m_pContext->ClearRenderTargetView(m_pTarget.Get(), colour);
}

void Graphics::DrawTriangle(float angle)
{
    HRESULT hr;

    struct Vertex
    {
        float x;
        float y;
        float z;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    // create vertex buffer (1 2d triangle at center of screen)
    const Vertex vertices[] =
    {
        {-1.0f, -1.0f, -1.0f    ,255, 0, 0},        //0
        {1.0f, -1.0f, -1.0f     ,0, 255, 0},        //1
        {-1.0f, 1.0f, -1.0f     ,0, 0, 255},        //2
        {1.0f, 1.0f, -1.0f      ,255, 255, 0},      //3

        {-1.0f, -1.0f, 1.0f     ,255, 0, 255},      //4
        {1.0f, -1.0f, 1.0f      ,0, 255, 255},      //5
        {-1.0f, 1.0f, 1.0f      ,0, 0, 0},          //6
        {1.0f, 1.0f, 1.0f       ,255, 255, 255},    //7
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC vDesc = {};
    vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vDesc.Usage = D3D11_USAGE_DEFAULT;      //default - values wont change once they are set
    vDesc.CPUAccessFlags = 0u;
    vDesc.MiscFlags = 0u;
    vDesc.ByteWidth = sizeof(vertices);
    vDesc.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA vSD = {};
    vSD.pSysMem = vertices;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&vDesc, &vSD, &vertexBuffer));

    // Bind vertex buffer to pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);



    // Create index buffer
    const unsigned short indices[] = 
    {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC inDesc = {};
    inDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    inDesc.Usage = D3D11_USAGE_DEFAULT;      //default - values wont change once they are set
    inDesc.CPUAccessFlags = 0u;
    inDesc.MiscFlags = 0u;
    inDesc.ByteWidth = sizeof(indices);
    inDesc.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA iSD = {};
    iSD.pSysMem = indices;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&inDesc, &iSD, &indexBuffer));

    // bind indices buffer to pipeline
    m_pContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


    // ********* WIP ********* //

    // create constant buffer for transform matrix
    struct ConstantBuffer
    {
        Matrix4F tranformation;
    };


    Vector3D quat{ 1, 1, 0 };
    Matrix4F rot = Matrix4F::CreateRotation(Quaternion::CreateFromAxisAngle(quat, angle));

    Matrix4F mov = Matrix4F::CreateTranslation(Vector3(0.0f, 0.0f, 4.0f));
    mov.Transpose();


    float width = 1.0f;
    float height = 3.0f / 4.0f;
    float NearZ = 0.5f;
    float FarZ = 10.0f;
    float twoNearZ = NearZ + NearZ;
    float fRange = FarZ / (FarZ - NearZ);

    Matrix4F perspective{twoNearZ / width, 0.0f, 0.0f,0.0f,
                         0.0f, twoNearZ / height, 0.0f, 0.0f,
                         0.0f, 0.0f, fRange, 1.0f,
                         0.0f, 0.0f, -fRange * NearZ, 0.0f};

    Matrix4F trans = { rot * mov * perspective };
    trans.Transpose();

    const ConstantBuffer cb{ trans };

    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    D3D11_BUFFER_DESC conDesc = {};
    conDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    conDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
    conDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    conDesc.MiscFlags = 0u;
    conDesc.ByteWidth = sizeof(cb);
    conDesc.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA cSD = {};
    cSD.pSysMem = &cb;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&conDesc, &cSD, &constantBuffer));

    //bind the buffer to the shader
    m_pContext->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

    // ********* WIP ********* //


    //**********MAKE SHADERS************//
    //create pixel shader
     Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
     Microsoft::WRL::ComPtr<ID3DBlob> blob;
     GFX_THROW_INFO(D3DReadFileToBlob(L"../Engine/Resources/Shaders/PixelShader.cso", &blob));
     GFX_THROW_INFO(m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
     m_pContext->PSSetShader(pixelShader.Get(), nullptr, 0u);
    
     //create vertex shader
     Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
     GFX_THROW_INFO(D3DReadFileToBlob(L"../Engine/Resources/Shaders/VertexShader.cso", &blob));
     GFX_THROW_INFO(m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
     m_pContext->VSSetShader(vertexShader.Get(), nullptr, 0u);
    //***********END SHADERS************//

    //bind render target
    m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf() , nullptr);

    //set primitive draw
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //create input layout
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    GFX_THROW_INFO(m_pDevice->CreateInputLayout(inputDesc,
        std::size(inputDesc),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &inputLayout));

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

    GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
}

void Graphics::DrawLoadedCube(std::string p_path)
{
    HRESULT hr;

    std::shared_ptr<ObjectElements::Model> mod;
    std::shared_ptr<ObjectElements::Mesh> mesh;
    mod = ObjectLoader::LoadModel(p_path);
	mesh = mod->GetMeshes()[0];

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC vDesc = {};
    vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vDesc.Usage = D3D11_USAGE_DEFAULT;
    vDesc.CPUAccessFlags = 0u;
    vDesc.MiscFlags = 0u;
    vDesc.ByteWidth = sizeof(mesh->m_vertices[0]) * mesh->m_vertices.size();
    vDesc.StructureByteStride = sizeof(Geometry::Vertex);

    D3D11_SUBRESOURCE_DATA vSD = {};
    vSD.pSysMem = mesh->m_vertices.data();
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&vDesc, &vSD, &vertexBuffer));
    const UINT stride = sizeof(Geometry::Vertex);
    const UINT offset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC inDesc = {};
    inDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    inDesc.Usage = D3D11_USAGE_DEFAULT;
    inDesc.CPUAccessFlags = 0u;
    inDesc.MiscFlags = 0u;
    inDesc.ByteWidth = sizeof(mesh->m_indices[0]) * mesh->m_indices.size();
    inDesc.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA iSD = {};
    iSD.pSysMem = mesh->m_indices.data();
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&inDesc, &iSD, &indexBuffer));
    m_pContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

    // ********* WIP ********* //

// create constant buffer for transform matrix
    struct ConstantBuffer
    {
        Matrix4F tranformation;
    };


    Vector3D quat{ 1, 1, 0 };
    Matrix4F rot = Matrix4F::CreateRotation(Quaternion::CreateFromAxisAngle(quat, 90));
	rot.Scale(Vector3F{ 0.02f,0.02f,0.02f });
    Matrix4F mov = Matrix4F::CreateTranslation(Vector3(0.0f, 0.0f, 4.0f));
    mov.Transpose();


    float width = 1.0f;
    float height = 3.0f / 4.0f;
    float NearZ = 0.5f;
    float FarZ = 10.0f;
    float twoNearZ = NearZ + NearZ;
    float fRange = FarZ / (FarZ - NearZ);

    Matrix4F perspective{ twoNearZ / width, 0.0f, 0.0f,0.0f,
                         0.0f, twoNearZ / height, 0.0f, 0.0f,
                         0.0f, 0.0f, fRange, 1.0f,
                         0.0f, 0.0f, -fRange * NearZ, 0.0f };

    Matrix4F trans = { rot * mov * perspective };
    trans.Transpose();

    const ConstantBuffer cb{ trans };

    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    D3D11_BUFFER_DESC conDesc = {};
    conDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    conDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
    conDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    conDesc.MiscFlags = 0u;
    conDesc.ByteWidth = sizeof(cb);
    conDesc.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA cSD = {};
    cSD.pSysMem = &cb;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&conDesc, &cSD, &constantBuffer));

    //bind the buffer to the shader
    m_pContext->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

    //**********MAKE SHADERS************//
    //create pixel shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    GFX_THROW_INFO(D3DReadFileToBlob(L"../Engine/Resources/Shaders/PixelShader.cso", &blob));
    GFX_THROW_INFO(m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
    m_pContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

    //create vertex shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"../Engine/Resources/Shaders/VertexShader.cso", &blob));
    GFX_THROW_INFO(m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
    m_pContext->VSSetShader(vertexShader.Get(), nullptr, 0u);
    //***********END SHADERS************//

        //bind render target
    m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), nullptr);

    //set primitive draw
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //create input layout
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    const D3D11_INPUT_ELEMENT_DESC inputDesc[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"Colour", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    GFX_THROW_INFO(m_pDevice->CreateInputLayout(inputDesc,
        std::size(inputDesc),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &inputLayout));

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

    GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed((UINT)mesh->m_indices.size(), 0u, 0u));

}

#pragma region ExceptionsClass

#pragma region HrExceptionClass
Graphics::HrException::HrException(int p_line, const char* p_file, HRESULT p_hr,
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

const char* Graphics::HrException::what() const noexcept
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

const char* Graphics::HrException::GetType() const noexcept
{
    return "Identity Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return m_hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(m_hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    char buf[512];
    DXGetErrorDescription(m_hr, buf, sizeof(buf));
    return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return m_info;
}
#pragma endregion

#pragma region InfoExceptionClass
Graphics::InfoException::InfoException(int p_line, const char* p_file, std::vector<std::string> p_infoMsg) noexcept
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

const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[ERROR INFO]\n" << GetErrorInfo() << std::endl << std::endl;
    oss << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
    return "Identity Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return m_info;
}
#pragma endregion

#pragma region DeviceExceptionClass
const char* Graphics::DeviceException::GetType() const noexcept
{
    return "Identity Graphics Exception [DEVICE REMOVED] (DXGI_ERROR_DEVICE_REMOVED)";
}
#pragma endregion

#pragma endregion