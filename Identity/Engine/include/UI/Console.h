#pragma once
#include <Export.h>
#include <cctype>
#include <cstdio>
#include <memory>
#include <Tools/ImGUI/imgui.h>

namespace Engine::UI
{
    //To write something in console, use AddLog() in Editor project (doesn't work from Engine::App)
    class API_ENGINE Console
    {
    public:
        Console();
        ~Console();

        // Portable helpers
        static int   Stricmp(const char* str1, const char* str2);
        static int   Strnicmp(const char* str1, const char* str2, int n);
        static char* Strdup(const char* str);
        static void  Strtrim(char* str);
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data); // In C++11 you are better off using lambdas for this sort of forwarding callbacks

        void    ClearLog();
        void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void    Draw();
        void    ExecCommand(const char* command_line);
        int     TextEditCallback(ImGuiInputTextCallbackData* data);

    private:
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        bool                  m_open = true;
        char                  InputBuf[256];
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImVector<char*>       Items;
        ImVector<const char*> Commands;
        ImVector<char*>       History;
        ImGuiTextFilter       Filter;
        std::string m_windowName = "Console";
    };
}
