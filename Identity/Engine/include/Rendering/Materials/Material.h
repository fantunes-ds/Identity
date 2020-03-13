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

        /**
         * @brief Used to create a new material from a pixel shader, a vertex shader and a texture
         * @param p_pixelShader The name of the pixel shader you want to use in the new material
         * @param p_vertexShader The name of the vertex shader you want to use in the new material
         * @return Return a share_ptr of the created material
         */
        static std::shared_ptr<Material> CreateMaterial(const std::shared_ptr<PixelShader> p_pixelShader, const std::shared_ptr<VertexShader> p_vertexShader);

        /**
         * @breif Used to add a pixel shader to the current material
         * @param p_device A pointer to the rendering device of the renderer (will soon be removed)
         * @param p_path The path of the pixel shader you want to add to the current material
         * @deprecated The Resource Manager will soon be available so prepare to make the change
         */
        void AddPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);

        /**
         * @brief Used to add a vertex shader to the current material
         * @param p_device A pointer to the rendering device of the renderer (will soon be removed)
         * @param p_path The pat of the vertex shader you want to add to the current material
         * @depracated The Resource Manager will soon be available so prepare to make the change
         */
        void AddVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);

        /**
         * @brief Used to add a texture to the current material
         * @param p_device A pointer to the rendering device of the renderer (will soon be removed)
         * @param p_path The path to the texture you want to add to the current material
         * @deprecated The Resource Manager will soon be available so prepare to make the change
         */
        void AddTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);

        /**
         * @brief Used to bind the shaders, the constant buffer and the texture to the rendering context
         * @param p_context A pointer to the device context of the renderer (will soon be removed)
         */
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        /**
         * @brief Used to unbind the shaders, the constant buffers and the texture from the rendering context
         * @param p_context A pointer to the device context of the renderer (will soon be removed)
         */
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
