#pragma once
#include <wrl/client.h>
#include <WinSetup.h>
#include <d3d11.h>

namespace Engine::Rendering::Buffers
{
    class AConstantBuffer
    {
    public :
        AConstantBuffer() = delete;
        virtual ~AConstantBuffer() = delete;

        
        virtual void Update(struct p_constantBuffer) = 0;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer{ nullptr };
    };
}
