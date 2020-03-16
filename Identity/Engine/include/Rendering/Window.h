#pragma once
#include <Export.h>
#include <WinSetup.h>
#include <Input/Keyboard.h>
#include <optional>
#include <Input/Mouse.h>

namespace Engine::Rendering
{
    /**
     * @brief The Windows32 window for the current device
     */
    class API_ENGINE Window
    {
    private:
        class API_ENGINE WindowClass
        {
        public:
            static const char* GetName();
            static HINSTANCE   GetInstance();

        private:
            WindowClass();
            ~WindowClass();
            WindowClass(const WindowClass&)            = delete;
            WindowClass& operator=(const WindowClass&) = delete;

            static constexpr const char* m_windowClassName = "Identity Engine";
            static WindowClass           m_windowClass;
            HINSTANCE                    m_hinstance;
        };

    public:
        /**
         * @brief Constructor that input width and height of the window, as well as a name for it.
         */
        Window(int p_width, int p_height, const char* p_name);
        ~Window();
        Window() = delete;

        /**
         * Redefines the title of the window.
         */
        void SetTitle(const std::string& title) const;

        /**
         * @brief Shows mouse cursor and clears all constrains.
         */
        void EnableCursor() noexcept;
        /**
         * @brief Hides mouse cursor and sets constrains.
         */
        void DisableCursor() noexcept;

        [[nodiscard]] bool GetIsCursorEnabled() const noexcept
        {
            return m_isCursorEnabled;
        }
        /**
         * @brief Check if there is a message in the queue
         * If there is no message, the method will return an empty optional and continue
         * If there is a message, the method will return the message
         */
        static std::optional<int> ProcessMessage();
    private:
        /**
         * @brief Sets cursors to be blocked inside the window.
         */
        void ConfineCursor() const noexcept;
        /**
         * @brief Let's cursor be free from the window.
         */
        void FreeCursor() const noexcept;
        /**
         * @brief Enables visibility for the cursor.
         */
        void ShowCursor() const noexcept;
        /**
         * @brief Disables visibility for the cursor.
         */
        void HideCursor() const noexcept;
        /**
         * @brief Enables usage of the mouse for ImGUI.
         */
        void EnableImGUIMouse() const noexcept;
        /**
         * @brief Disables usage of the mouse for ImGUI.
         */
        void DisableImGUIMouse() const noexcept;

        static LRESULT CALLBACK HandleMsgSetup(const HWND   p_hwnd, const UINT p_msg,
                                               const WPARAM p_wParam, const LPARAM p_lParam);
        static LRESULT CALLBACK HandleMsgThunk(const HWND   p_hwnd, const UINT p_msg,
                                               const WPARAM p_wParam,const LPARAM p_lParam);
        LRESULT HandleMsg(HWND p_hwnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);

        int  m_width;
        int  m_height;
        bool m_isCursorEnabled;
        HWND m_hwnd;

        std::vector<char> m_rawBuffer;

        bool isSet = false;
    };
}
