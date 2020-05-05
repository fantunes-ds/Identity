#include <stdafx.h>

#include <Tools/ImGUI/imgui.h>

#include <Core/App.h>
#include <Rendering/Renderer.h>
#include <UI/Dockspace.h>


using namespace Engine::UI;

void Dockspace::CreateDockspace(Core::App& p_appRef)
{
    static bool               opt_fullscreen_persistant = true;
    bool                      opt_fullscreen            = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags           = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    static bool test = true;
    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &test, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO(); (void)io;


    // DockSpace
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        CreateMenuBar(p_appRef);
        
    }
    ImGui::End();
}

void Dockspace::CreateMenuBar(Core::App& p_appRef)
{
    if (ImGui::BeginMenuBar())
    {
        //todo all shortcuts
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("New Scene", "Ctrl + N", nullptr);
            ImGui::MenuItem("Open Scene", "Ctrl + O", nullptr);
            ImGui::Separator();
            ImGui::MenuItem("Save", "Ctrl + S", nullptr);
            ImGui::MenuItem("Save As...", "Ctrl + Shift + S", nullptr);
            ImGui::MenuItem("Build", "Ctrl + B", nullptr);
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", nullptr, nullptr))
            {
                p_appRef.SetIsApplicationRunning(false);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Play", "Ctrl + P", nullptr))
            {
                p_appRef.TestingSimulation();
            }
            if (ImGui::MenuItem("Stop", "Ctrl + O", nullptr))
            {
                p_appRef.TestingSimulation();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::BeginMenu("Create"))
            {
                ImGui::MenuItem("Folder", "", nullptr);
                ImGui::Separator();
                ImGui::MenuItem("Scene", "", nullptr);
                ImGui::Separator();
                ImGui::MenuItem("Material", "", nullptr);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObject"))
        {
            ImGui::MenuItem("Create Empty", "Ctrl + Shift + N", nullptr);
            if (ImGui::BeginMenu("3D Object"))
            {
                ImGui::MenuItem("Cube", "", nullptr);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Light"))
            {
                ImGui::MenuItem("Directional Light", "", nullptr);
                ImGui::EndMenu();
            }

            ImGui::MenuItem("Camera", "", nullptr);
            ImGui::EndMenu();
        }

        ImGui::SetCursorPosX(Rendering::Renderer::GetInstance()->GetWidth() / 2);
        if (ImGui::Button("Play"))
        {
            p_appRef.TestingSimulation();
        }

        ImGui::EndMenuBar();
    }
}
