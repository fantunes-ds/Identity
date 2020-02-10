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

        template <typename T>
        void Update(const T& p_filledBuffer);

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        PCB m_pcb;
    };
}
