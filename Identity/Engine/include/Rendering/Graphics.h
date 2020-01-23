#pragma once
#include <Export.h>

#include <WinSetup.h>
#include <vector>
#include <d3d11.h>
#include <Tools/IdentityException.h>
#include <wrl.h>

namespace Engine::Rendering
{
    /*
     @brief Contains the DirectX API
     */
    class API_ENGINE Graphics
    {
        //All exception class for the Graphics class
    public:
        class Exception : public Tools::IdentityException
        {
            using IdentityException::IdentityException;
        };

        /*
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
        /*
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
        /*
         @brief Get the exceptions for the Graphics driver
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
        Graphics(const HWND p_hwnd);
        Graphics(const Graphics&) = delete;
        Graphics& operator=(const Graphics&) = delete;
        ~Graphics();

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