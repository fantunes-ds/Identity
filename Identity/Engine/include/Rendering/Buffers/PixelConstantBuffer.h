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
        Vector3F                            cameraPos;
        float                               textureState;
    };

    class PixelConstantBuffer
    {
    public:
        PixelConstantBuffer()  = default;
        ~PixelConstantBuffer() = default;

        /**
         * @brief Sets parameters and creates the buffers for the pixelConstantBuffer (PCB).
         */
        void GenBuffers();
        /**
         * @brief Attaches the buffer to the current render call.
         */
        void Bind() const;
        /**
         * @brief Detaches the buffer from the current render call.
         */
        void Unbind() const;

        /**
         * @brief returns the structure that is used in the buffer
         * @warning : NOT THE ACTUAL BUFFER, JUST THE STRUCTURE FOR IT.
         */
        [[nodiscard]] const PCB& GetPCB() const { return m_pcb; }
        /**
         * @brief returns the buffer of the PCB.
         */
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() const { return m_buffer; }

        /**
         * Applies new changes to the PixelConstantBuffer
         */
        void Update(const PCB& p_filledBuffer) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        PCB                                  m_pcb;
    };
}
