#pragma once
#include <Export.h>

#include <vector>
#include <d3d11.h>
#include <Tools/IdentityException.h>
#include <wrl.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/RenderTexture.h>

namespace Engine::Rendering
{
    /*
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
        Renderer(const HWND& p_hwnd, const int& p_clientWidth, const int& p_clientHeight);
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        ~Renderer() = default;

        static void InitRenderer(const HWND p_hwnd, const int p_clientWidth, const int p_clientHeight);
        static const std::unique_ptr<Renderer>& GetInstance() noexcept { return instance; }
        /*
         @brief Switch the front buffer with the back buffer
         */
        void EndFrame() const;
        /*
         @brief Reset the base colour of the back buffer
         */
        void ClearBuffers(const float& p_red, const float& p_green, const float& p_blue) const;

        /*
        @brief Set the renderer to fullscreen and call the Resize method
        */
        void SetFullscreen(const bool& p_state);
        /*
         @brief Resize the renderer using new resolution.
        */
        void Resize(const float& p_width, const float& p_height);

        void GetResolution(int& p_width, int& p_height);

        [[nodiscard]] const bool& GetFullscreenState() const { return isFullscreen; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() const { return m_pDevice; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<IDXGISwapChain>& GetSwapChain() const { return m_pSwapChain; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetContext() const { return m_pContext; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetTarget() const { return m_pTarget; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() const { return m_pDepthStencilView; }
        [[nodiscard]] const Microsoft::WRL::ComPtr<ID3DBlob>& GetBlob() const { return m_blob; }
        [[nodiscard]] const float& GetWidth() const { return m_width; }
        [[nodiscard]] const float& GetHeight() const { return m_height; }
        [[nodiscard]] const std::vector<RenderTexture>& GetRenderTextures() const { return m_renderTextures; }

    private:
        /*
         @brief Reset the context so we can resize it
         */
        void ResetContext();
        /*
         @brief Create the SwapChain using the handle to the window
         */
        void CreateSwapChain(const HWND& p_hwnd);
        /*
         @brief Set the Depth and the Stencil buffers. Need to be done when resizing
         */
        void SetDepthStencilBuffers();
        /*
         @brief Set the viewport of the renderer
         */
        void SetViewPort(const float& p_width, const float& p_height) const;
        /*
         @brief Initialize the back buffer and link it to the SwapChain
         */
        void SetBackBuffer();

        /*
         @brief Change the fullscreen resolution
         */
        void ChangeResolution();

        void CreateRenderTexture();

        Microsoft::WRL::ComPtr<ID3D11Device>           m_pDevice;
        Microsoft::WRL::ComPtr<IDXGISwapChain>         m_pSwapChain;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_pContext;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
        Microsoft::WRL::ComPtr<ID3DBlob>               m_blob;
        std::vector<RenderTexture>                     m_renderTextures;


        bool isFullscreen = false;
        bool m_enable4xMSAA = false;
        UINT m_4xMsaaQuality{ 0 };
        float m_width{ 0 };
        float m_height{ 0 };
        float m_fullWidth{ 1920.f };
        float m_fullHeight{ 1080.f };
        static std::unique_ptr<Renderer> instance;
    };
}
