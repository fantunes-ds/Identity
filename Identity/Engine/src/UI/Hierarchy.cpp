#include <stdafx.h>

#include <Tools/ImGUI/imgui.h>

#include <Managers/SceneManager.h>
#include <Scene/Scene.h>
#include <UI/Hierarchy.h>

#include "Core/App.h"

using namespace Engine::UI;

std::shared_ptr<Engine::Scene::SceneNode> Hierarchy::DisplayNextChild(std::shared_ptr<Scene::SceneNode> p_child, int& p_i)
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int                selection_mask = (1 << 2);
    int                       node_clicked = -1;
    ImGuiTreeNodeFlags        node_flags = base_flags;
    static bool               test_drag_and_drop = true;
    const bool                is_selected = (selection_mask & (1 << p_i)) != 0;
    if (is_selected)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    if (!p_child->GetChildren().empty())
    {
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)p_i, node_flags, "%s", p_child->GetName().c_str());
        {
            if (ImGui::IsItemClicked())
                node_clicked = p_i;
            if (test_drag_and_drop && ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("%s", p_child->GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if (node_open)
            {
                for (auto& child : p_child->GetChildren())
                    DisplayNextChild(child, p_i);
                ImGui::TreePop();
            }
            p_i++;
        }
    }
    else
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)p_i, node_flags, "%s", p_child->GetName().c_str());
        if (ImGui::IsItemClicked())
            node_clicked = p_i;
        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("%s", p_child->GetName().c_str());
            ImGui::EndDragDropSource();
        }
        p_i++;
    }
    if (node_clicked != -1)
    {
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        else if (!(selection_mask & (1 << node_clicked)))
            selection_mask = (1 << node_clicked);           // Click to single-select
    }

    return p_child;
}

void Hierarchy::CreateHierarchy(Core::App& p_appRef)
{
    if (ImGui::Begin("Hierarchy"))
    {
        int i = 0;
        for (auto& node : Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetRootSceneNodes())
            DisplayNextChild(node.second, i);
    }
    ImGui::End();
}