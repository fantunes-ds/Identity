#pragma once

#include <Export.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <Geometry/Vertex.h>

namespace Engine::Rendering::Buffers
{
    struct API_ENGINE IndexBuffer
    {
        /**
         * @brief Sets parameters and creates the buffers for the indexBuffer (EBO).
         * @param p_indices : The indices of the buffer.
         */
        void Generate(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, std::vector<unsigned short> p_indices);
        /**
         * @brief Attaches the buffer to the current render call.
         */
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;
        /**
         * @brief Detaches the buffer from the current render call.
         */
        void Unbind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context) const;

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() { return m_buffer; }
        [[nodiscard]] D3D11_SUBRESOURCE_DATA& GetSubData() { return m_subData; }
        [[nodiscard]] D3D11_BUFFER_DESC& GetDescriptor() { return m_descriptor; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        D3D11_SUBRESOURCE_DATA m_subData;
        D3D11_BUFFER_DESC m_descriptor;
    };
}
