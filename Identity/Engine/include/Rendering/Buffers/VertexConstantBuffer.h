#pragma once
#include <wrl/client.h>
#include <WinSetup.h>
#include <d3d11.h>


namespace Engine::Rendering::Buffers
{
    struct VCB
    {
        Matrix4F model;
        Matrix4F view;
        Matrix4F normalModel;
        Matrix4F projection;
    };
    class VertexConstantBuffer
    {
    public:
        VertexConstantBuffer() = default;
        ~VertexConstantBuffer() = default;

        void GenBuffers();

        const VCB& GetVCB() const { return m_vcb; }

        template <typename T>
        void Update(const T& p_filledBuffer);

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        VCB m_vcb;
    };
}
