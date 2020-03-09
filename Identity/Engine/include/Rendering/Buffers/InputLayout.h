#pragma once

#include <Export.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace Engine::Rendering::Buffers
{
    struct API_ENGINE InputLayout
    {
        void Generate(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const Microsoft::WRL::ComPtr<ID3DBlob>& p_blob);
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputDesc;
    };
}
