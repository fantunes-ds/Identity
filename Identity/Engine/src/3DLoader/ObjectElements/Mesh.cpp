#include <stdafx.h>

#include <3DLoader/ObjectElements/Mesh.h>
#include <Tools/DirectX/GraphicsMacros.h>
#include <WinSetup.h>
#include <d3dcompiler.h>

Engine::ObjectElements::Mesh::Mesh(std::vector<Engine::Geometry::Vertex>& p_vertices, std::vector<unsigned short>& p_indices) :
    m_vertices { p_vertices }, m_indices { p_indices } {}

Engine::ObjectElements::Mesh::Mesh(const Mesh& p_other):
    m_vertices { p_other.m_vertices }, m_indices { p_other.m_indices } {}

void Engine::ObjectElements::Mesh::GenerateBuffers(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device)
{
    LoadPixelShader(p_device, L"../Engine/Resources/Shaders/PixelShader.cso");
    LoadVertexShader(p_device, L"../Engine/Resources/Shaders/VertexShader.cso");
    m_vertexBuffer.Generate(p_device, m_vertices);
    m_indexBuffer.Generate(p_device, m_indices);
    m_inputLayout.Generate(p_device, blob_test);

    Vector3D quat{ 0, 1, 0 };
    transform = Matrix4F::CreateTransformation(Vector3F(0.0f, 0.0f, 0.0f),
        Quaternion::CreateFromAxisAngle(quat, GPM::Tools::Utils::ToRadians(0.0f)),
        Vector3F{ 0.02f, 0.02f, 0.02f });

}

void Engine::ObjectElements::Mesh::Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    BindShader(p_context);
    m_vertexBuffer.Bind(p_context);
    m_indexBuffer.Bind(p_context);
    m_inputLayout.Bind(p_context);
}

bool Engine::ObjectElements::Mesh::operator==(const Mesh& p_other) const
{
    if (m_vertices.size() != p_other.m_vertices.size() || m_indices != p_other.m_indices)
        return false;

    if (m_vertexBuffer == p_other.m_vertexBuffer)
        return true;

    return false;
}

bool Engine::ObjectElements::Mesh::operator!=(const Mesh& p_other) const
{
    if (m_vertices.size() != p_other.m_vertices.size() || m_indices != p_other.m_indices)
        return true;

    if (m_vertexBuffer == p_other.m_vertexBuffer)
        return false;

    return true;
}

void Engine::ObjectElements::Mesh::LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &blob_test));
    GFX_THROW_INFO(p_device->CreatePixelShader(blob_test->GetBufferPointer(), blob_test->GetBufferSize(), nullptr, &pixelShader));
}

void Engine::ObjectElements::Mesh::LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path)
{
    HRESULT hr;

    GFX_THROW_INFO(D3DReadFileToBlob(p_path.c_str(), &blob_test));
    GFX_THROW_INFO(p_device->CreateVertexShader(blob_test->GetBufferPointer(), blob_test->GetBufferSize(), nullptr, &vertexShader));
}

void Engine::ObjectElements::Mesh::BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context)
{
    p_context->PSSetShader(pixelShader.Get(), nullptr, 0u);
    p_context->VSSetShader(vertexShader.Get(), nullptr, 0u);
}
