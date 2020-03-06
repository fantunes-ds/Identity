#include <stdafx.h>
#include <Systems/RenderSystem.h>
#include <Containers/ModelContainer.h>
#include "Tools/DirectX/GraphicsMacros.h"
#include "Rendering/Lights/Light.h"
#include "Tools/ImGUI/imgui.h"
#include "Tools/ImGUI/imgui_impl_dx11.h"
#include <Input/Input.h>
#include <Containers/GameObjectContainer.h>
#include <Containers/TransformContainer.h>
#include "Components/ModelComponent.h"
#include "Containers/CameraContainer.h"
#include <Containers/EventContainer.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>
#include <Containers/LightContainer.h>
#include <Scene/SceneGraph/SceneNode.h>
#include "Tools/Bullet/BulletDebugRender.h"

//WIP

//Example of how to use events
Engine::Systems::RenderSystem::RenderSystem()
{
}

void Engine::Systems::RenderSystem::DrawScene(float p_deltaTime)
{
    HRESULT hr;
    Rendering::Renderer::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    std::shared_ptr<Rendering::Lights::ILight> ILight = Containers::LightContainer::GetLights().begin()->second;
    std::shared_ptr<Rendering::Lights::Light> light1 = std::dynamic_pointer_cast<Rendering::Lights::Light>(Containers::LightContainer::GetLights().begin()->second);
    Rendering::Lights::Light::LightData& light = light1->GetLightData();

    std::shared_ptr<Rendering::Camera> camera = Containers::CameraContainer::GetCamera(m_activeCamera);

    float* pos[3] = { &light.position.x, &light.position.y, &light.position.z };

    //TODO: Light will be moved soon
    if (ImGui::Begin("Lighting Tool"))
    {
        ImGui::DragFloat3("LightPos", *pos, 0.1f, -90.0f, 90.0f, "%.1f");
        ImGui::SliderFloat("LightColR", &light.color.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("LightColG", &light.color.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("LightColB", &light.color.z, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("SpecColR", &light.specular.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("SpecColG", &light.specular.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("SpecColB", &light.specular.z, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightX", &light.ambient.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightY", &light.ambient.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightZ", &light.ambient.z, 0.0f, 1.0f, "%.1f");
    }ImGui::End();

    for (auto& sceneNode : Scene::SceneGraph::GetInstance()->GetRootSceneNodes())
    {
        DrawSceneNode(sceneNode.second);
    }

    //TEST
    auto mesh = Containers::ModelContainer::FindModel(Containers::ModelContainer::FindModel("DebugBox"))->GetMeshes()[0];
    mesh->SetMaterial(Containers::MaterialContainer::FindMaterial("LinkTexture"));
    mesh->GenerateBuffers(Rendering::Renderer::GetInstance()->GetDevice());

    mesh->Bind(Rendering::Renderer::GetInstance()->GetContext());

    mesh->GetMaterial().GetShader().GenConstantBuffers();

    Matrix4F model = Tools::Bullet::BulletDebugRenderer::GetInstance()->GetWorldMatrix();
    //model.Transpose();
    Matrix4F normalModel = Matrix4F::Inverse(model);

    Matrix4F view = camera->GetViewMatrix();
    Matrix4F perspective = camera->GetPerspectiveMatrix();
    perspective.Transpose();


    Rendering::Buffers::VCB vcb{ model, view, normalModel,perspective };
    mesh->GetMaterial().GetShader().GetVCB().Update(vcb);
    const Vector3F cameraPos = camera->GetPosition();

    const Vector4F reversedXLightPos = Vector4F(light.position.x, light.position.y, -light.position.z, 1.0f);
    const Rendering::Buffers::PCB pcb{ reversedXLightPos, light.ambient, light.diffuse,
                                        light.specular , light.color,
                                                        light.shininess,Vector3F{},Vector3{cameraPos.x, cameraPos.y, cameraPos.z}, 0.0f };
    mesh->GetMaterial().GetShader().GetPCB().Update(pcb);
    Rendering::Renderer::GetInstance()->SetRenderTarget();
    Rendering::Renderer::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

    GFX_THROW_INFO_ONLY(Rendering::Renderer::GetInstance()->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));
}

void Engine::Systems::RenderSystem::DrawSceneNode(std::shared_ptr<Scene::SceneNode> p_sceneNode)
{
    auto camera = Containers::CameraContainer::GetCamera(m_activeCamera);
    auto mesh = p_sceneNode->GetMesh();
    std::shared_ptr<Rendering::Lights::Light> light1 = std::dynamic_pointer_cast<Rendering::Lights::Light>(Containers::LightContainer::GetLights().begin()->second);
    Rendering::Lights::Light::LightData& light = light1->GetLightData();

    if (mesh != nullptr)
    {
        mesh->Bind(Rendering::Renderer::GetInstance()->GetContext());

        mesh->GetMaterial().GetShader().GenConstantBuffers();

        Matrix4F model = Containers::TransformContainer::FindTransform(p_sceneNode->GetTransform())->GetWorldTransformMatrix();

        Matrix4F normalModel = Matrix4F::Inverse(model);

        Matrix4F view = camera->GetViewMatrix();
        Matrix4F perspective = camera->GetPerspectiveMatrix();
        perspective.Transpose();


        Rendering::Buffers::VCB vcb{ model, view, normalModel,perspective };
        mesh->GetMaterial().GetShader().GetVCB().Update(vcb);
        const Vector3F cameraPos = camera->GetPosition();

        const Vector4F reversedXLightPos = Vector4F(light.position.x, light.position.y, -light.position.z, 1.0f);
        const Rendering::Buffers::PCB pcb{ reversedXLightPos, light.ambient, light.diffuse,
                                            light.specular , light.color,
                                                            light.shininess,Vector3F{},Vector3{cameraPos.x, cameraPos.y, cameraPos.z}, 0.0f };
        mesh->GetMaterial().GetShader().GetPCB().Update(pcb);
        Rendering::Renderer::GetInstance()->SetRenderTarget();

        GFX_THROW_INFO_ONLY(Rendering::Renderer::GetInstance()->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));
    }

    for (auto child : p_sceneNode->GetChildren())
    {
        DrawSceneNode(child);
    }
}

void Engine::Systems::RenderSystem::Update(float p_deltaTime)
{
    DrawScene(p_deltaTime);
    Scene::SceneGraph::GetInstance()->UpdateScene(0);

    if (Containers::CameraContainer::GetCamera(m_activeCamera))
    {
        int width, height;
        Rendering::Renderer::GetInstance()->GetResolution(width, height);
        Containers::CameraContainer::GetCamera(m_activeCamera)->UpdateCamera(width, height);
    }
}

void Engine::Systems::RenderSystem::ResetActiveCamera()
{
    m_activeCamera = -1;
}

void Engine::Systems::RenderSystem::SetActiveCamera(int32_t p_id)
{
    m_activeCamera = p_id;
}
