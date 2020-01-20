#pragma once
#include <Export.h>

#include <WinSetup.h>
#include <d3d11.h>
#include <wrl.h>

namespace Engine::Rendering
{
    /*
     @brief Contains the DirectX API
     */
    class API_ENGINE Graphics
    {
    public:
        Graphics(const HWND p_hwnd);
        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;
        ~Graphics() = default;

        /*
         @brief Switch the front buffer with the back buffer
         */
        void EndFrame();
        /*
         @brief Reset the base colour of the back buffer
         */
        void ClearBuffer(float p_red, float p_green, float p_blue);


        void DrawTriangle();

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
    };
}