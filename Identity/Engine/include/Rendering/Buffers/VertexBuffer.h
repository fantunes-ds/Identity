#pragma once

#include <Export.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <Geometry/Vertex.h>

namespace Engine::Rendering::Buffers
{
    struct API_ENGINE VertexBuffer
    {
        void Generate(const Microsoft::WRL::ComPtr<ID3D11Device>& p_device, std::vector<Geometry::Vertex>& p_vertices);
        void Bind(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& p_context);

        bool operator==(const VertexBuffer& p_other) const;

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() { return m_buffer; }
        [[nodiscard]] D3D11_SUBRESOURCE_DATA& GetSubData() { return m_subData; }
        [[nodiscard]] D3D11_BUFFER_DESC& GetDescriptor() { return m_descriptor; }
        [[nodiscard]] UINT& GetStrides() { return m_stride; }
        [[nodiscard]] UINT& GetOffset() { return m_offset; }

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        D3D11_SUBRESOURCE_DATA m_subData;
        D3D11_BUFFER_DESC m_descriptor;
        UINT m_stride;
        UINT m_offset;
    };
}
