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
        void Generate(Microsoft::WRL::ComPtr<ID3D11Device> p_device, std::vector<Geometry::Vertex> p_vertices);
        void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context);

        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        D3D11_SUBRESOURCE_DATA subData;
        D3D11_BUFFER_DESC descriptor;
        UINT stride;
        UINT offset;
    };
}
