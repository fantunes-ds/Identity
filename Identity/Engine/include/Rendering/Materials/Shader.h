#pragma once
#include <WinSetup.h>
#include <wrl/client.h>
#include <d3d11.h>

namespace Engine::Rendering::Materials
{
    class Shader
    {
    public:
        void LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        const Microsoft::WRL::ComPtr<ID3DBlob>& GetBlob() const { return blob_test; }

    private:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3DBlob> blob_test;
    };
}
