#pragma once
#include <wrl/client.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <Rendering/Light.h>

namespace Engine::Rendering::Buffers
{
    struct PCB
    {
        Rendering::Light lightSource;
        Vector3F cameraPos;
    };
    class PixelConstantBuffer
    {
    public:
        PixelConstantBuffer() = default;
        ~PixelConstantBuffer() = default;

        void GenBuffers();

        const PCB& GetVCB() const { return m_pcb; }

        void Update(const PCB& p_filledBuffer) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        PCB m_pcb;
    };
}
