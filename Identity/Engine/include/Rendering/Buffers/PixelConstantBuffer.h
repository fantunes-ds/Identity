#pragma once
#include <wrl/client.h>
#include <WinSetup.h>
#include <d3d11.h>
#include <Rendering/Lights/Light.h>

namespace Engine::Rendering::Buffers
{
    struct PCB
    {
        Rendering::Lights::Light::LightData lightSource;
        Vector3F cameraPos;
        float padding;
    };
    class PixelConstantBuffer
    {
    public:
        PixelConstantBuffer() = default;
        ~PixelConstantBuffer() = default;

        void GenBuffers();
        void Bind() const;
        void Unbind() const;

        [[nodiscard]] const PCB& GetPCB() const { return m_pcb; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() const { return m_buffer; }

        void Update(const PCB& p_filledBuffer) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        PCB m_pcb;
    };
}
