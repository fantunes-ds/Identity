#include <stdafx.h>
#include <Tools/ImGUI/ImGUIManager.h>
#include <Tools/ImGUI/imgui.h>


Engine::Tools::UI::ImGUIManager::ImGUIManager()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
}

Engine::Tools::UI::ImGUIManager::~ImGUIManager()
{
    ImGui::DestroyContext();
}
