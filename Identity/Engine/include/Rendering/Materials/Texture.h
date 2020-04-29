#pragma once

#include <WinSetup.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include <utility>
#include <Objects/IObject.h>


namespace Engine::Rendering::Materials
{
    class Texture : public Objects::IObject
    {
    public:
        Texture();
        ~Texture() = default;

        inline void SetPath(const std::string& p_path) { m_path = p_path; }
        [[nodiscard]] inline const std::string& GetPath() const { return m_path; }

        /**
         * @brief Create a new Texture from the path and the name given
         * @param p_path The path of the texture to load
         * @param p_name The name you want to give to the texture
         * @return Return a shared_ptr of the created texture
         */
        static std::shared_ptr<Texture> LoadTexture(const std::string& p_path, const std::string& p_name);

        /**
         * @brief Bind the texture to the rendering context
         */
        void BindTexture();

        /**
         * @brief Used to unbind texture from the rendering context
         * @param p_context A pointer to the device context of the renderer (will soon be removed)
         */
        void UnbindTexture();

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureShaderResourceView() const { return m_textureShaderResourceView; }
        void SetTextureShaderResourceView(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_textureShaderResourceView) { m_textureShaderResourceView = p_textureShaderResourceView; }
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampleState() const { return m_samplerState; }

    private:
        std::string m_path{};
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureShaderResourceView;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;
        Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
    };
}
