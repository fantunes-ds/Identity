#include <stdafx.h>
#include <Systems/RenderSystem.h>
#include <Containers/ModelContainer.h>
#include "Tools/DirectX/GraphicsMacros.h"
#include "Rendering/Light.h"
#include "Tools/ImGUI/imgui.h"
#include "Tools/ImGUI/imgui_impl_dx11.h"
#include <Input/Input.h>
#include <Containers/GameObjectContainer.h>
#include <Containers/TransformContainer.h>
#include "Components/ModelComponent.h"
#include "Containers/CameraContainer.h"
#include <Containers/EventContainer.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>

//Example of how to use events
Engine::Systems::RenderSystem::RenderSystem(Rendering::Renderer* p_renderer) : m_renderer(p_renderer)
{
    /*Containers::EventContainer::AddEvent("NoActiveCamera");
    Event& event = Containers::EventContainer::GetEvent("NoActiveCamera");
    event.AddListener(this, &RenderSystem::ResetActiveCamera);*/
}

void Engine::Systems::RenderSystem::DrawScene()
{
    if (!m_renderer)
    {
        std::string error("in Engine::Systems::RenderSystem::DrawScene(): cannot draw scene because Renderer* m_renderer is nullptr");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return;
    }

    HRESULT hr;

    std::shared_ptr<Rendering::Light> light = m_lights.begin()->second;
    std::shared_ptr<Rendering::Camera> camera = Containers::CameraContainer::GetCamera(m_activeCamera);

    //TODO: Light will be moved soon
    if (ImGui::Begin("Lighting Tool"))
    {
        ImGui::SliderFloat("LightPosX", &light->position.x, -40.0f, 40.0f, "%.1f");
        ImGui::SliderFloat("LightPosY", &light->position.y, -40.0f, 40.0f, "%.1f");
        ImGui::SliderFloat("LightPosZ", &light->position.z, -40.0f, 40.0f, "%.1f");
        ImGui::SliderFloat("LightColR", &light->color.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("LightColG", &light->color.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("LightColB", &light->color.z, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightX", &light->ambient.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightY", &light->ambient.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightZ", &light->ambient.z, 0.0f, 1.0f, "%.1f");
    }ImGui::End();

    for (auto& gameObject : Containers::GameObjectContainer::GetAllGameObjects())
    {
        for (auto& component : gameObject.second->GetAllComponents())
        {
            if (std::shared_ptr<Components::ModelComponent> modelComp = std::dynamic_pointer_cast<Components::ModelComponent>(Containers::ComponentContainer::FindComponent(component)))
            {
                auto& meshes = Containers::ModelContainer::FindModel(modelComp->GetModel())->GetMeshes();

                for (auto mesh : meshes)
                {
                    mesh->Bind(m_renderer->GetContext());
                    
                    Matrix4F model = Containers::TransformContainer::FindTransform(gameObject.second->GetTransformID())->GetTransformMatrix();

                    Matrix4F normalModel = Matrix4F::Inverse(model);

                    Matrix4F view = camera->GetViewMatrix();
                    Matrix4F perspective = camera->GetPerspectiveMatrix();

                    model.Transpose();
                    view.Transpose();
                    normalModel.Transpose();
                    perspective.Transpose();

                    const Rendering::Buffers::VCB vcb { model, view, normalModel,perspective };
                    mesh->GetMaterial().GetShader().GetVCB().Update(vcb);

                    const Vector4F reversedXLightPos = Vector4F(light->position.x * -1, light->position.y, light->position.z, 1.0f);
                    const Rendering::Buffers::PCB pcb { reversedXLightPos, light->ambient, light->diffuse,
                                                        light->specular ,light->direction, light->color,
                                                                        light->shininess,camera->GetPosition()};
                    mesh->GetMaterial().GetShader().GetPCB().Update(pcb);
                    
                    GFX_THROW_INFO_ONLY(m_renderer->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));
                }
            }
        }
    }
}

void Engine::Systems::RenderSystem::Update()
{
    DrawScene();

    if (Containers::CameraContainer::GetCamera(m_activeCamera))
    {
        Containers::CameraContainer::GetCamera(m_activeCamera)->UpdateCamera();
    }
}


void Engine::Systems::RenderSystem::UpdateCamera() const
{
    int width, height;
    m_renderer->GetResolution(width, height);
    Containers::CameraContainer::GetCamera(m_activeCamera)->UpdateResolution(width, height);
}

uint32_t Engine::Systems::RenderSystem::AddLight(Rendering::Light& p_light)
{
    //TODO: create lightManager and load light into it rather than into rendersystem
    std::shared_ptr newLight = std::make_shared<Rendering::Light>(p_light);

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
