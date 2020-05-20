#include <stdafx.h>

#include <Tools/ImGUI/imgui.h>

#include <Managers/SceneManager.h>
#include <Scene/Scene.h>
#include <UI/Hierarchy.h>

#include <Core/App.h>
#include <Rendering/Renderer.h>
#include <Components/BoxCollider.h>

#include "Components/Camera.h"
#include "Components/Light.h"

int Engine::UI::Hierarchy::m_currentlySelected = -1;

std::shared_ptr<Engine::Scene::SceneNode> Engine::UI::Hierarchy::DisplayNextChild(std::shared_ptr<Scene::SceneNode> p_child)
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int                selection_mask = (1 << 2);
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
                m_currentlySelected = p_child->GetID();
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
            m_currentlySelected = p_child->GetID();
        if (test_drag_and_drop && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
            ImGui::Text("%s", p_child->GetName().c_str());
            ImGui::EndDragDropSource();
        }
    }
    if (m_currentlySelected != -1)
    {
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << m_currentlySelected);          // CTRL+click to toggle
        else if (!(selection_mask & (1 << m_currentlySelected)))
            selection_mask = (1 << m_currentlySelected);           // Click to single-select

    }

    return p_child;
}

void Engine::UI::Hierarchy::CreateHierarchy(Core::App& p_appRef)
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

void Engine::UI::Hierarchy::CallInspector(int32_t p_id)
{
    ImGui::Begin("Inspector");

    if (p_id < 0)
    {
        ImGui::End();
        return;
    }

    //todo Class Inspector to deal with these

    auto transform = Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetAllSceneNodes().find(p_id)->second->GetGameObject()->GetTransform();

    Quaternion& rotationQuaternion = transform->GetRotation();
    Vector3F    rotationEuler      = { static_cast<float>(rotationQuaternion.ToEuler().x),
                                       static_cast<float>(rotationQuaternion.ToEuler().y),
                                       static_cast<float>(rotationQuaternion.ToEuler().z)
    };
    if (ImGui::CollapsingHeader("Transform"))
    {
        float* pos[3]   = {&transform->GetPosition().x, &transform->GetPosition().y, &transform->GetPosition().z};
        float* rot[3]   = {&rotationEuler.x, &rotationEuler.y, &rotationEuler.z};
        float* scale[3] = {&transform->GetScale().x, &transform->GetScale().y, &transform->GetScale().z};
        ImGui::DragFloat3("Position", *pos, 0.1f);
        ImGui::DragFloat3("Rotation", *rot, 0.1f);
        ImGui::DragFloat3("Scale", *scale, 0.1f);
    }

    if (rotationEuler.y > 90.0f || rotationEuler.y < -90.0f)
        rotationEuler = Vector3{rotationEuler.x - 180.0f, ((rotationEuler.y) * -1), rotationEuler.z + 180.0f};

    rotationQuaternion.MakeFromEuler(rotationEuler);

    transform->needUpdate     = true;
    transform->needAxesUpdate = true;

    for (auto component : Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetAllSceneNodes().find(p_id)->second->GetGameObject()->GetAllComponents())
    {
        std::shared_ptr<Components::IComponent> Icomponent = Containers::ComponentContainer::FindComponent(component);
        switch (Icomponent->GetType())
        {
        case Components::MODEL:
            {
                std::shared_ptr<Components::ModelComponent> modelComponent = std::dynamic_pointer_cast<Components::ModelComponent>(Icomponent);

                if (ImGui::CollapsingHeader("Model Component"))
                {
                    ImGui::Text("%s", Managers::ResourceManager::FindModel(modelComponent->GetModel())->GetName().c_str());
                }

                if (ImGui::CollapsingHeader("Material"))
                {
                    std::shared_ptr<Rendering::Materials::Material> mat = modelComponent->GetMaterial();

                    //float* diffuse[3] = { &.diffuse.x, &lightData.diffuse.y, &lightData.diffuse.z };
                    //ImGui::ColorPicker3("Light Color", *color);
                    ImGui::Text("Texture");
                    ImGui::Image(*mat->GetTexture()->GetTextureShaderResourceView().GetAddressOf(), ImVec2(100, 100));
                }
                break;
            }
        case Components::BOX_COLLIDER:
            {
                std::shared_ptr<Components::BoxCollider> boxCollider = std::dynamic_pointer_cast<Components::BoxCollider>(Icomponent);
                if (ImGui::CollapsingHeader("Box Collider"))
                {
                    //todo add dimensions, it's weirdly done on BoxCollider for the moment being.
                    float* dimensions[3] = {&boxCollider->GetDimensions().x, &boxCollider->GetDimensions().y, &boxCollider->GetDimensions().z};
                    float* offset[3] = {&boxCollider->GetOffset().x, &boxCollider->GetOffset().y, &boxCollider->GetOffset().z};
                    float* mass = {&boxCollider->GetMass()};

                    ImGui::DragFloat3("Dimensions", *dimensions, 0.1f);
                    ImGui::DragFloat("Mass", mass, 0.1f);
                    ImGui::DragFloat3("Offset", *offset, 0.1f);

                    boxCollider->SetPositionOffset(boxCollider->GetOffset());
                    boxCollider->SetMass(boxCollider->GetMass());
                    boxCollider->SetDimensions(boxCollider->GetDimensions());
                }
                break;
            }
        case Components::CAMERA:
            {
                std::shared_ptr<Components::Camera> camera = std::dynamic_pointer_cast<Components::Camera>(Icomponent);
                if (ImGui::CollapsingHeader("Camera"))
                {
                    float fov = camera->GetFOV();

                    ImGui::SliderFloat("Camera FOV", &fov, 10.f, 180.f, "%1.f");

                    camera->SetFOV(fov);
                }
                break;
            }
        case Components::LIGHT:
            {
                std::shared_ptr<Components::Light> lightComp = std::dynamic_pointer_cast<Components::Light>(Icomponent);
                std::shared_ptr<Rendering::Lights::ILight> ILight = lightComp->GetLight();
                Rendering::Lights::ILight::LightData& lightData = ILight->GetLightData();
                if (ImGui::CollapsingHeader("Light"))
                {
                    float* ambient[3] = { &lightData.ambient.x, &lightData.ambient.y, &lightData.ambient.z };
                    float* color[3] = { &lightData.color.x, &lightData.color.y, &lightData.color.z };
                    float* diffuse[3] = { &lightData.diffuse.x, &lightData.diffuse.y, &lightData.diffuse.z };
                    float* specular[3] = { &lightData.specular.x, &lightData.specular.y, &lightData.specular.z };
                    float* shininess = { &lightData.shininess };
                    ImGui::ColorPicker3("Ambient Light Color", *ambient);
                    ImGui::ColorPicker3("Light Color", *color);
                    ImGui::ColorPicker3("Diffuse Light Color", *diffuse);
                    ImGui::ColorPicker3("Specular Light Color", *specular);
                    ImGui::DragFloat("shininess", shininess, 0.1f);
                }
                break;
            }
        case Components::UNSET:
            {
                OutputDebugString("Component Type not Set for Inspector Call. Please set the Component Type on your Component to something valid.");
                break;
            }
        }
    }
    ImGui::End();
}
