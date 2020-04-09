#pragma once

#include <WinSetup.h>
#include <wrl/client.h>
#include <d3d11.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>
#include <Rendering/Buffers/PixelConstantBuffer.h>

namespace Engine::Rendering::Materials
{
    class Shader
    {
    public:
        Shader();
        ~Shader() = default;

        /**
         * @brief Creates a pixelShader from p_path.
         * @param p_path the shader file to link.
         */
        void LoadPixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        /**
         * @brief Creates a vertexShader from p_path.
         * @param p_path the shader file to link.
         */
        void LoadVertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, const std::wstring& p_path);
        /**
         * @brief Attaches the already configured shader to the current render call.
         * @warning Only call this when the shader has been properly set. You need a vertexShader and a PixelShader.
         */
        void BindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        /**
         * @brief Detaches the buffer from the current render call.
         */
        void UnbindShader(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        /**
         * @brief Calls for the creation of the the vertex and pixel constant buffers (VCB/PCB)
         */
        void GenConstantBuffers();
        /**
         * @brief Attaches the vertex and pixel constant buffers (VCB/PCB) to the current render call.
         */
        void BindConstantBuffers(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        /**
         * @brief Detaches the vertex and pixel constant buffers (VCB/PCB) to the current render call.
         */
        void UnbindConstantBuffers(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;

        const Microsoft::WRL::ComPtr<ID3DBlob>& GetBlob() const { return m_blob; }

        const Buffers::VertexConstantBuffer& GetVCB() const noexcept { return m_vcb; }
        const Buffers::PixelConstantBuffer& GetPCB() const noexcept { return m_pcb; }

    private:
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

        Buffers::VertexConstantBuffer m_vcb;
        Buffers::PixelConstantBuffer m_pcb;
        
        Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
    };
}
