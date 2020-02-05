#pragma once
#include <Export.h>

#include <WinSetup.h>
#include <vector>
#include <d3d11.h>
#include <Tools/IdentityException.h>
#include <wrl.h>
#include "3DLoader/ObjectElements/Model.h"
#include "Camera.h"

namespace Engine::Rendering
{
    /**
     @brief Contains the DirectX API
     */
    class API_ENGINE Renderer
    {
        //All exception class for the Renderer class
    public:
        class Exception : public Tools::IdentityException
        {
            using IdentityException::IdentityException;
        };

        /**
         @brief Get the exceptions for the functions wich returns an HRESULT
         */
        class HrException : public Exception
        {
        public:
            HrException(int p_line, const char* p_file, HRESULT p_hr, std::vector<std::string> p_infoMsg = {}) noexcept;
            const char* what() const noexcept override;
            const char* GetType() const noexcept override;
            HRESULT GetErrorCode() const noexcept;
            std::string GetErrorString() const noexcept;
            std::string GetErrorDescription() const noexcept;
            std::string GetErrorInfo() const noexcept;
        private:
            HRESULT m_hr;
            std::string m_info;
        };
        /**
         @brief Get the information when the functions return a void
         */
        class InfoException : public Exception
        {
        public:
            InfoException(int p_line, const char* p_file, std::vector<std::string> p_infoMsg) noexcept;
            const char* what() const noexcept override;
            const char* GetType() const noexcept override;
            std::string GetErrorInfo() const noexcept;
        private:
            std::string m_info;
        };
        /**
         @brief Get the exceptions for the Renderer driver
         */
        class DeviceException : public HrException
        {
            using HrException::HrException;
        public:
            const char* GetType() const noexcept override;
        private:
            std::string m_reason;
        };

    public:
        Renderer(const HWND p_hwnd, const int p_clientWidth, const int p_clientHeight);
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        ~Renderer();

        /**
         @brief Switch the front buffer with the back buffer
         */
        void EndFrame() const;
        /**
         @brief Reset the base colour of the back buffer
         */
        void ClearBuffer(float p_red, float p_green, float p_blue) const;

        //---------WIP--------

        void LoadPixelShader(const std::wstring& p_path);
        void LoadVertexShader(const std::wstring& p_path);

        void Resize(const int p_width, const int p_height);
        void GetResolution(int& p_width, int& p_height);

        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() { return m_pDevice; };
        [[nodiscard]] Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain() { return m_pSwapChain; };
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetContext() { return m_pContext; };
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetTarget() { return m_pTarget; };
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencil() { return m_pDepthStencil; };
        [[nodiscard]] Microsoft::WRL::ComPtr<ID3DBlob>& GetBlob() { return m_blob; };

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencil;
        Microsoft::WRL::ComPtr<ID3DBlob> m_blob;


        int m_width;
        int m_height;
        UINT m_4xMsaaQuality;
        bool m_enable4xMSAA = true;
    };
}
