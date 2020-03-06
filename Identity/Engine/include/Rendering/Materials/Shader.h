#pragma once
#include <WinSetup.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>
#include <Rendering/Buffers/PixelConstantBuffer.h>

namespace Engine::Rendering::Materials
{
    class Shader
    {
    public:
        Shader();
        ~Shader() = default;
        void LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        void UnbindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        void BindConstantBuffers(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        void UnbindConstantBuffers(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;

        void GenConstantBuffers();

        const Microsoft::WRL::ComPtr<ID3DBlob>& GetBlob() const { return blob_test; }

        const Buffers::VertexConstantBuffer& GetVCB() const noexcept { return m_vcb; }
        const Buffers::PixelConstantBuffer& GetPCB() const noexcept { return m_pcb; }

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

        Buffers::VertexConstantBuffer m_vcb;
        Buffers::PixelConstantBuffer m_pcb;
        
        Microsoft::WRL::ComPtr<ID3DBlob> blob_test;
    };
}
