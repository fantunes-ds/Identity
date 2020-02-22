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

        void LoadTexture(const std::string& p_path);
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() const { return m_texture; }

    private:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_text;
    };
}
