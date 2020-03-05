#include <stdafx.h>
#include <Systems/RenderSystem.h>
#include <Containers/ModelContainer.h>
#include <Tools/DirectX/GraphicsMacros.h>
#include <Rendering/Lights/Light.h>
#include <Tools/ImGUI/imgui.h>
#include <Input/Input.h>
#include <Containers/GameObjectContainer.h>
#include <Containers/TransformContainer.h>
#include <Components/ModelComponent.h>
#include <Containers/CameraContainer.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>
#include <Containers/LightContainer.h>

void Engine::Systems::RenderSystem::DrawScene()
{
    HRESULT hr;

    std::shared_ptr<Rendering::Lights::Light>  light1 = std::dynamic_pointer_cast<Rendering::Lights::Light>(Containers::LightContainer::GetLights().begin()->second);
    Rendering::Lights::Light::LightData& light = light1->GetLightData();

    std::shared_ptr<Rendering::Camera> camera = Containers::CameraContainer::GetCamera(m_activeCamera);

    for (auto& gameObject : Containers::GameObjectContainer::GetAllGameObjects())
    {
        for (auto& component : gameObject.second->GetAllComponents())
        {
            if (std::shared_ptr<Components::ModelComponent> modelComp = std::dynamic_pointer_cast<Components::ModelComponent
            >(Containers::ComponentContainer::FindComponent(component)))
            {
                auto& meshes = Containers::ModelContainer::FindModel(modelComp->GetModel())->GetMeshes();

                for (auto mesh : meshes)
                {
                    mesh->Bind(Rendering::Renderer::GetInstance()->GetContext());
                    mesh->GetMaterial().GetShader().GenConstantBuffers();

                    Matrix4F model = Containers::TransformContainer::FindTransform(gameObject.second->GetTransformID())->
                            GetTransformMatrix();

                    Matrix4F normalModel = Matrix4F::Inverse(model);

                    Matrix4F view        = camera->GetViewMatrix();
                    Matrix4F perspective = camera->GetPerspectiveMatrix();

                    normalModel.Transpose();
                    perspective.Transpose();

                    Rendering::Buffers::VCB vcb{model, view, normalModel, perspective};
                    mesh->GetMaterial().GetShader().GetVCB().Update(vcb);

                    const Vector4F reversedXLightPos = Vector4F(light.position.x * -1, light.position.y, light.position.z, 1.0f);
                    const Rendering::Buffers::PCB pcb{
                        reversedXLightPos, light.ambient, light.diffuse,
                        light.specular, light.color,
                        light.shininess, Vector3F{}, camera->GetPosition(), 0.0f
                    };
                    mesh->GetMaterial().GetShader().GetPCB().Update(pcb);

                    for (auto& RT : Rendering::Renderer::GetInstance()->GetRenderTextures())
                        RT.Bind();
                    GFX_THROW_INFO_ONLY(Rendering::Renderer::GetInstance()->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));
                }
            }
        }
        if (ImGui::Begin("DirectionInfo"))
        {
            ImGui::Text("Forward: %f | %f | %f", gameObject.second->GetTransform()->GetForward().x, gameObject.second->GetTransform()->GetForward().y, gameObject.second->GetTransform()->GetForward().z);
            ImGui::Text("Up: %f | %f | %f", gameObject.second->GetTransform()->GetUp().x, gameObject.second->GetTransform()->GetUp().y, gameObject.second->GetTransform()->GetUp().z);
            ImGui::Text("Right: %f | %f | %f", gameObject.second->GetTransform()->GetRight().x, gameObject.second->GetTransform()->GetRight().y, gameObject.second->GetTransform()->GetRight().z);
            ImGui::Text("--------------", gameObject.second->GetTransform()->GetRight().x, gameObject.second->GetTransform()->GetRight().y, gameObject.second->GetTransform()->GetRight().z);
        }ImGui::End();
    }
    static bool open = true;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | !ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::Begin("Scene", &open, window_flags);
    ImGui::SetScrollX(ImGui::GetScrollMaxX() * 0.5f);
    ImGui::SetScrollY(ImGui::GetScrollMaxY() * 0.5f);
    ImGui::Image(Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetShaderResourceView().Get(), ImVec2(Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetRect().x,
                                                                                                                                  Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetRect().y));
    ImGui::End();
}

void Engine::Systems::RenderSystem::Update()
{
    DrawScene();

    if (Containers::CameraContainer::GetCamera(m_activeCamera))
    {
        int width, height;
        Rendering::Renderer::GetInstance()->GetResolution(width, height);
        Containers::CameraContainer::GetCamera(m_activeCamera)->UpdateCamera(width, height);
    }
}

uint32_t Engine::Systems::RenderSystem::AddLight(Rendering::Lights::Light& p_light)
{
    //TODO: create lightManager and load light into it rather than into rendersystem
    std::shared_ptr newLight = std::make_shared<Rendering::Lights::Light>(p_light);

    if (newLight)
    {
        const uint32_t tmpId = Tools::IDCounter::GetNewID();
        m_lights.insert_or_assign(tmpId, newLight);
        return tmpId;
    }
    else
        return -1;
}

void Engine::Systems::RenderSystem::ResetActiveCamera()
{
    m_activeCamera = -1;
}

void Engine::Systems::RenderSystem::SetActiveCamera(int32_t p_id)
{
    m_activeCamera = p_id;
}
