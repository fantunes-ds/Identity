#pragma once
#include <Rendering/Materials/Shader.h>
#include <Rendering/Materials/Texture.h>
#include <Objects/IObject.h>

namespace Engine::Rendering
{
    class Material : public Objects::IObject
    {
    public:
        Material() = default;
        ~Material() = default;

        void AddPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void AddVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void AddTexture(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        [[nodiscard]] Materials::Shader& GetShader() { return m_shader; }

        void SetName(const std::string& p_name) { m_name = p_name; }
        [[nodiscard]] const std::string& GetName() const { return m_name; }

    private:
        std::string m_name;
        Materials::Shader m_shader;
        Materials::Texture m_texture;
    };
}
