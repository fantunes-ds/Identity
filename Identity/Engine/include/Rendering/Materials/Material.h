#pragma once
#include <Rendering/Materials/Shader.h>
#include <Rendering/Materials/Texture.h>
#include <Objects/IObject.h>
#include "PixelShader.h"
#include "VertexShader.h"

namespace Engine::Rendering::Materials
{
    class Material : public Objects::IObject
    {
    public:
        Material() = default;
        ~Material() = default;

        static std::shared_ptr<Material> CreateMaterial(const std::shared_ptr<PixelShader> p_pixelShader, const std::shared_ptr<VertexShader> p_vertexShader);

        void AddPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void AddVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void AddTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);

        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);
        void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        inline void SetPixelShader(const std::shared_ptr<PixelShader> p_pixelShader) { m_pixelShader = p_pixelShader; }
        inline void SetVertexShader(const std::shared_ptr<VertexShader> p_vertexShader) { m_vertexShader = p_vertexShader; }
        inline void SetTexture(const std::shared_ptr<Texture> p_texture) { m_textureWIP = p_texture; }
        [[nodiscard]] inline const std::shared_ptr<PixelShader> GetPixelShaderWIP() const { return m_pixelShader; }
        [[nodiscard]] inline const std::shared_ptr<VertexShader> GetVertexShaderWIP() const { return m_vertexShader; }
        [[nodiscard]] inline const std::shared_ptr<Texture> GetTextureWIP() const { return m_textureWIP; }

        [[nodiscard]] inline Texture& GetTexture() { return m_texture; }
        [[nodiscard]] inline Shader& GetShader() { return m_shader; }

    private:
        std::string m_name{};
        Shader m_shader{};
        Texture m_texture{};

        //WIP
        std::shared_ptr<Texture> m_textureWIP{};
        std::shared_ptr<PixelShader> m_pixelShader{};
        std::shared_ptr<VertexShader> m_vertexShader{};
        //---
    };
}
