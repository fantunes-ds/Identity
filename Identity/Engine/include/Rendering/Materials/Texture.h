#pragma once

#include <WinSetup.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include <Objects/IObject.h>


namespace Engine::Rendering::Materials
{
    class Texture : public Objects::IObject
    {
    public:
        Texture();
        ~Texture();

        inline void SetPath(const std::string& p_path) { m_path = p_path; }
        [[nodiscard]] inline const std::string& GetPath() const { return m_path; }

        //WIP
        static std::shared_ptr<Texture> LoadTexture(const std::string& p_path, const std::string& p_name);
        //-----


        void LoadTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void BindTexture(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture() const { return m_texture; }
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampleState() const { return m_samplerState; }

    private:
        std::string m_path{};
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
        Microsoft::WRL::ComPtr<ID3D11Resource> m_text;
    };
}
