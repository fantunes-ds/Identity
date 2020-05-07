#include <stdafx.h>

#include <Tools/ImGUI/imgui.h>

#include <Managers/SceneManager.h>
#include <Scene/Scene.h>
#include <UI/Hierarchy.h>

#include <Core/App.h>
#include <Rendering/Renderer.h>

using namespace Engine::UI;

int Hierarchy::m_currentlySelected = -1;

std::shared_ptr<Engine::Scene::SceneNode> Hierarchy::DisplayNextChild(std::shared_ptr<Scene::SceneNode> p_child)
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int                selection_mask = (1 << 2);
    int                       node_clicked = -1;
    ImGuiTreeNodeFlags        node_flags = base_flags;
    static bool               test_drag_and_drop = true;
    const bool                is_selected = (selection_mask & (1 << p_child->GetID())) != 0;


    if (is_selected)
        node_flags |= ImGuiTreeNodeFlags_Selected;

    if (!p_child->GetChildren().empty())
    {
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)p_child->GetID(), node_flags, "%s", p_child->GetName().c_str());
        {
            if (ImGui::IsItemClicked())
                node_clicked = p_child->GetID();
            if (test_drag_and_drop && ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("%s", p_child->GetName().c_str());
                ImGui::EndDragDropSource();
            }
            if (node_open)
            {
                for (auto& child : p_child->GetChildren())
                    DisplayNextChild(child);
                ImGui::TreePop();
            }
        }
    }
    else
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)p_child->GetID(), node_flags, "%s", p_child->GetName().c_str());
        if (ImGui::IsItemClicked())
            node_clicked = p_child->GetID();
        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("%s", p_child->GetName().c_str());
            ImGui::EndDragDropSource();
        }
    }
    if (node_clicked != -1)
    {
        m_currentlySelected = node_clicked;
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
            DisplayNextChild(node.second);
    }
    ImGui::End();
    CallInspector(m_currentlySelected);
}

void Hierarchy::CallInspector(int32_t p_id)
{
    ImGui::Begin("Inspector");

    if (p_id < 0)
    {
        ImGui::End();
        return;
    }   
    //for (auto& gameObject : Managers::SceneManager::GetActiveScene()->GetAllGameObjectsInScene())
    //{
        
        //for (auto component : gameObject->GetAllComponents())
        auto transform = Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetAllSceneNodes().find(p_id)->second->GetGameObject()->GetTransform();


        if (ImGui::CollapsingHeader("Transform"))
        {
            float* pos[3] = { &transform->GetPosition().x, &transform->GetPosition().y, &transform->GetPosition().z };
            float rot[3] = { transform->GetRotation().ToEuler().x, transform->GetRotation().ToEuler().y, transform->GetRotation().ToEuler().z };
            float* scale[3] = { &transform->GetScale().x, &transform->GetScale().y, &transform->GetScale().z };
            ImGui::DragFloat3("Position", *pos, 0.1f, -1000.0f,1000.0f, "%.3f");
            ImGui::DragFloat3("Rotation", rot, 0.1f);
            ImGui::DragFloat3("Scale", *scale, 0.1f);
        }
            transform->needUpdate = true;
        for (auto component : Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetAllSceneNodes().find(p_id)->second->GetGameObject()->GetAllComponents())
        {

            std::shared_ptr<Components::IComponent> Icomponent = Containers::ComponentContainer::FindComponent(component);
            switch (Icomponent->GetType())
            {
            case Components::MODEL:
            {
                std::shared_ptr<Components::ModelComponent> t = std::dynamic_pointer_cast<Components::ModelComponent>(Icomponent);
                
                if (ImGui::CollapsingHeader("Model Component"))
                {
                    ImGui::Text("%s", Managers::ResourceManager::FindModel(t->GetModel())->GetName().c_str());
                }
            }
            break;
            case Components::BOX_COLLIDER:

                break;
            case Components::CAMERA:

                break;
            case Components::LIGHT:

                break;
            case Components::UNSET:
                OutputDebugString("Component Type not Set for Inspector Call. Please set the Component Type on your Component to something valid.");
                break;
            }
        }
    //}
        ImGui::End();
}