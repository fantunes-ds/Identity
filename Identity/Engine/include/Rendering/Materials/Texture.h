#pragma once

#include <WinSetup.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>

namespace Engine::Rendering::Materials
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        void LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void BindTexture(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexSRV() const { return m_texSRV; }
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampleState() const { return m_samplerState; }

    private:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texSRV;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
        Microsoft::WRL::ComPtr<ID3D11Resource> m_text;
    };
}
