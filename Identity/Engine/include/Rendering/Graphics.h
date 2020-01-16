#pragma once
#include <Export.h>

#include <WinSetup.h>
#include <d3d11.h>

namespace Engine::Rendering
{
    class API_ENGINE Graphics
    {
    public:
        Graphics(HWND p_hwnd);
        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;
        ~Graphics();

        void EndFrame();
        void ClearBuffer(float p_red, float p_green, float p_blue);

    private:
        ID3D11Device* m_pDevice = nullptr;
        IDXGISwapChain* m_pSwapChain = nullptr;
        ID3D11DeviceContext* m_pContext = nullptr;
        ID3D11RenderTargetView* m_pTarget = nullptr;
    };
}