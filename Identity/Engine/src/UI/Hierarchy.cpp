#include <stdafx.h>

#include <Tools/ImGUI/imgui.h>

#include <Core/App.h>
#include <Components/BoxCollider.h>
#include <Components/SphereCollider.h>
#include <Components/Camera.h>
#include <Components/Light.h>
#include <Components/Sound.h>
#include <Managers/SceneManager.h>
#include <Rendering/Renderer.h>
#include <Scene/Scene.h>
#include <UI/Hierarchy.h>

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
        ImGui::DragFloat3("Position", *pos, 0.1f,0,0,"%0.2f");
        ImGui::DragFloat3("Rotation", *rot, 0.1f, 0, 0, "%0.2f");
        ImGui::DragFloat3("Scale", *scale, 0.1f, 0, 0, "%0.2f");
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
                    if (const auto model = Managers::ResourceManager::FindModel(modelComponent->GetModel()))
                        ImGui::Text("Current model : %s", model->GetName().c_str());

                    if (ImGui::BeginPopup("Select Mesh"))
                    {
                        ImGui::Text("Choose Model");
                        for (auto& model : Managers::ResourceManager::GetAllModels())
                        {
                            if (model->GetName() == "NoName")
                                continue;

                            if (ImGui::Button(model->GetName().c_str()))
                            {
                                modelComponent->SetModel(model->GetID());
                            }
                        }ImGui::Button("Add new model");

                        ImGui::EndPopup();
                    }

                    if (ImGui::Button("Change Model"))
                    {
                        ImGui::OpenPopup("Select Mesh");
                    }
                }

                if (ImGui::CollapsingHeader("Material"))
                {
                    std::shared_ptr<Rendering::Materials::Material> mat = modelComponent->GetMaterial();

                    if (ImGui::BeginPopup("Select Material"))
                    {
                        ImGui::Text("Choose Material");
                        for (auto& material : Managers::ResourceManager::GetAllMaterials())
                        {
                            if (material->GetName() == "NoName" || material->GetName() == "RenderText")
                                continue;

                            if (ImGui::Button(material->GetName().c_str()))
                            {
                                modelComponent->SetMaterial(material->GetName());
                            }
                        }

                        ImGui::EndPopup();
                    }

                    if (ImGui::Button("Change Material"))
                    {
                        ImGui::OpenPopup("Select Material");
                    }

                    if (mat == nullptr)
                        break;

                    if (mat->GetName()._Equal("default"))
                    {
                        ImGui::TextWrapped("Using default material. Please add a new material to edit it.");
                        break;
                    }
                    float* objectColor[3] = { &mat->GetColor().x, &mat->GetColor().y, &mat->GetColor().z };
                    ImGui::ColorEdit3("Light Color", *objectColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    if (mat->GetTextureState() == true)
                    {
                        ImGui::Text("Texture");
                        ImGui::Image(*mat->GetTexture()->GetTextureShaderResourceView().GetAddressOf(), ImVec2(100, 100));
                    }

                    if (ImGui::BeginPopup("Select Texture"))
                    {
                        ImGui::Text("Choose Texture");
                        if (ImGui::Button("None"))
                        {
                            modelComponent->GetMaterial()->SetTexture(nullptr);
                            modelComponent->GetMaterial()->SetTextureState(false);
                        }
                        for (auto& texture : Managers::ResourceManager::GetAllTextures())
                        {
                            if (texture->GetName() == "NoName")
                                continue;
                            
                            if (ImGui::Button(texture->GetName().c_str()))
                            {
                                modelComponent->GetMaterial()->SetTexture(texture);
                            }
                        }
                        ImGui::Button("Add new texture");

                        ImGui::EndPopup();
                    }

                    if (ImGui::Button("Change Texture"))
                    {
                        ImGui::OpenPopup("Select Texture");
                    }
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

                    ImGui::DragFloat3("Dimensions", *dimensions, 0.1f, 0, 0, "%0.2f");
                    ImGui::DragFloat("Mass", mass, 0.1f, 0, 0, "%0.2f");
                    ImGui::DragFloat3("Offset", *offset, 0.1f, 0, 0, "%0.2f");

                    boxCollider->SetPositionOffset(boxCollider->GetOffset());
                    boxCollider->SetMass(boxCollider->GetMass());
                    boxCollider->SetDimensions(boxCollider->GetDimensions());
                }
                break;
            }
        case Components::SPHERE_COLLIDER:
        {
            std::shared_ptr<Components::SphereCollider> sphereCollider = std::dynamic_pointer_cast<Components::SphereCollider>(Icomponent);
            if (ImGui::CollapsingHeader("Sphere Collider"))
            {
                //todo add dimensions, it's weirdly done on BoxCollider for the moment being.
                float* radius = { &sphereCollider->GetRadius() };
                float* offset[3] = { &sphereCollider->GetOffset().x, &sphereCollider->GetOffset().y, &sphereCollider->GetOffset().z };
                float* mass = { &sphereCollider->GetMass() };

                ImGui::DragFloat("Radius", radius, 0.1f, 0, 0, "%0.2f");
                ImGui::DragFloat("Mass", mass, 0.1f, 0, 0, "%0.2f");
                ImGui::DragFloat3("Offset", *offset, 0.1f, 0, 0, "%0.2f");

                sphereCollider->SetPositionOffset(sphereCollider->GetOffset());
                sphereCollider->SetMass(sphereCollider->GetMass());
                sphereCollider->SetRadius(sphereCollider->GetRadius());
            }
            break;
        }
        case Components::CAMERA:
            {
                std::shared_ptr<Components::Camera> camera = std::dynamic_pointer_cast<Components::Camera>(Icomponent);
                if (ImGui::CollapsingHeader("Camera"))
                {
                    float fov = camera->GetFOV();

                    ImGui::SliderFloat("Camera FOV", &fov, 10.f, 180.f, "%0.f");

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
                    float* ambient[4] = { &lightData.ambient.x, &lightData.ambient.y, &lightData.ambient.z, &lightData.ambient.w };
                    float* diffuse[4] = { &lightData.diffuse.x, &lightData.diffuse.y, &lightData.diffuse.z, &lightData.diffuse.w };
                    float* specular[4] = { &lightData.specular.x, &lightData.specular.y, &lightData.specular.z, &lightData.specular.w };
                    float* range = { &lightData.range };
                    float* shininess = { &lightData.shininess };
                    float* radius = { &lightData.range };

                    ImGui::ColorEdit3("Ambient Color", *ambient, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    ImGui::SameLine();
                    ImGui::Text("Ambient Color");
                    ImGui::SetNextItemWidth(100);
                    ImGui::SliderFloat("Ambient intensity", &lightData.ambient.w, 0.00f, 1.0f, "%.2f");
                    ImGui::Separator();
                    ImGui::ColorEdit3("Diffuse Light Color", *diffuse, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    ImGui::SetNextItemWidth(100);
                    ImGui::SliderFloat("Diffuse intensity", &lightData.diffuse.w, 0.00f, 1.0f, "%.2f");
                    ImGui::Separator();
                    ImGui::ColorEdit3("Specular Light Color", *specular, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                    ImGui::SetNextItemWidth(100);
                    ImGui::SliderFloat("Specular intensity", &lightData.specular.w, 0.00f, 1.0f, "%.2f");
                    ImGui::Separator();
                    ImGui::SetNextItemWidth(100);
                    ImGui::SliderFloat("Shininess", shininess, 8.0f, 512.0f, "%.0f");
                    ImGui::SliderFloat("Range", range, 0.00f, 1.0f, "%.2f");
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
    Vector2F windowSize{ ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
    ImGui::SetCursorPosX((windowSize.x - 200) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);

    if (ImGui::BeginPopup("Add Component"))
    {
        std::vector<std::string> types {"BOX_COLLIDER",
                                        "LIGHT",
                                        "CAMERA",
                                        "MODEL",
                                        "SOUND",
                                        "SPHERE_COLLIDER" };
        for (unsigned int i = 0; i < types.size(); ++i)
        {
            if (ImGui::Button(types[i].c_str(), ImVec2(200,20)))
            {
                auto gameObject = Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetAllSceneNodes().find(p_id)->second->GetGameObject();
                const Vector2F size(1920, 1080);
                switch (i)
                {
                case 0:
                    gameObject->AddComponent<Components::BoxCollider>();
                    break;
                case 1:
                    gameObject->AddComponent<Components::Light>();
                    break;
                case 2:
                    gameObject->AddComponent<Components::Camera>(size.x, size.y);
                    break;
                case 3:
                    gameObject->AddComponent<Components::ModelComponent>();
                    break;
                case 4:
                    gameObject->AddComponent<Components::Sound>();
                    break;
                case 5:
                    gameObject->AddComponent<Components::SphereCollider>();
                    break;
                }
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Add component", ImVec2(200, 20)))
    {
        ImGui::SetCursorPosX(ImGui::GetWindowPos().x);
        ImGui::OpenPopup("Add Component");
    }

    ImGui::End();
}
