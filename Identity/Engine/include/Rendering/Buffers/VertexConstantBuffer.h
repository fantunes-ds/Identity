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

        [[nodiscard]] const VCB& GetVCB() const { return m_vcb; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetBuffer() const { return m_buffer; }

        void Update(const VCB& p_filledBuffer) const;

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
        VCB m_vcb;
    };
}
