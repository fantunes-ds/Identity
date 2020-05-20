#pragma once
#include <Rendering/Materials/Shader.h>
#include <Rendering/Materials/Texture.h>
#include <Objects/IObject.h>
#include "PixelShader.h"
#include "VertexShader.h"

namespace Engine::Rendering::Materials
{
    class Material : public Objects::IObject, public Serialize::ISerializeable
    {
    public:
        Material();
        ~Material() = default;

        /**
         * @brief Used to create a new material from a pixel shader, a vertex shader and a texture
         * @param p_pixelShader The name of the pixel shader you want to use in the new material
         * @param p_vertexShader The name of the vertex shader you want to use in the new material
         * @return Return a share_ptr of the created material
         */
        static std::shared_ptr<Material> CreateMaterial(const std::shared_ptr<PixelShader> p_pixelShader, const std::shared_ptr<VertexShader> p_vertexShader);

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
        inline void SetTexture(const std::shared_ptr<Texture> p_texture) { m_texture = p_texture; m_textureState = true; }
        inline void SetTextureState(const bool p_state) { m_textureState = p_state; }
        inline void SetColor(const Vector3F p_color) { m_color = p_color; }

        [[nodiscard]] inline const Vector3F GetColor() const { return m_color; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3DBlob> GetBlob();
        [[nodiscard]] inline const std::shared_ptr<PixelShader> GetPixelShader() const { return m_pixelShader; }
        [[nodiscard]] inline const std::shared_ptr<VertexShader> GetVertexShader() const { return m_vertexShader; }
        [[nodiscard]] inline const std::shared_ptr<Texture> GetTexture() const { return m_texture; }
        [[nodiscard]] inline const bool GetTextureState() const { return m_textureState; }


        void Serialize(std::ostream& p_stream) override;
        void Unserialize(std::istream& p_stream) override {}

    private:
        bool m_textureState{ false };

        Vector3F m_color{ 1.0f, 1.0f, 1.0f};
        std::shared_ptr<Texture> m_texture{nullptr};
        std::shared_ptr<PixelShader> m_pixelShader{};
        std::shared_ptr<VertexShader> m_vertexShader{};
    };
}
