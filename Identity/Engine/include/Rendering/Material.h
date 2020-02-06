#pragma once
#include "Materials/Shader.h"

namespace Engine::Rendering
{
    class Material
    {
    public:
        void AddPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void AddVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        [[nodiscard]] const Materials::Shader& GetShader() const { return m_shader; }

    private:
        Materials::Shader m_shader;
    };
}
