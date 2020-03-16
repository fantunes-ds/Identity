#pragma once

#include <Export.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace Engine::Rendering
{
    class API_ENGINE RenderTexture
    {
    public :
        RenderTexture()  = delete;
        ~RenderTexture() = default;

        /**
         * @brief Creates a RenderTexture. It includes a Texture2D, linked to a RenderTargetView and writing on a ShaderResourceView.
         * @param p_width : the width of our Rendered Texture.
         * @param p_height : the height of our Rendered Texture.
         * @param p_createDepthBuffer : Decide whether or not to create a unique Depth buffer
         * for our Texture. By default this option is off, but you probably want to turn it on.
         */
        RenderTexture(UINT p_width, UINT p_height, bool p_createDepthBuffer = false);

        /**
         * @brief Creates a RenderTexture. It includes a Texture2D, linked to a RenderTargetView and writing on a ShaderResourceView.
         * @param p_width : the width of our Rendered Texture.
         * @param p_height : the height of our Rendered Texture.
         * @param p_stencil : the depthStencil buffer to be bound to the renderViewTarget.
         */
        RenderTexture(UINT p_width, UINT p_height, Microsoft::WRL::ComPtr<ID3D11DepthStencilView> p_stencil);

        /** 
         * Binds the RenderTargetView and DepthStencilView (if there is one) to the OutputMerger.
         */
        void Bind() const;

        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return m_renderShaderResourceView; }

        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetTarget() const { return m_renderTargetView; }

        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const { return m_stencilView; }

        [[nodiscard]] const Vector2F GetRect() const { return Vector2U{m_width, m_height}; }


    private:
        void CreateTexture(UINT p_width, UINT p_height);
        void CreateDepthStencilBuffer(UINT p_width, UINT p_height);

        Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_renderTargetTexture;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   m_renderTargetView;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_renderShaderResourceView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   m_stencilView;
        UINT                                             m_width{}, m_height{};
    };
}
