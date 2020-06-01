#include <stdafx.h>

#include <Tools/DirectX/GraphicsMacros.h>
#include <Tools/ImGUI/imgui.h>

#include <Components/BoxCollider.h>
#include <Components/SphereCollider.h>
#include <Input/Input.h>
#include <Managers/ResourceManager.h>
#include <Managers/SceneManager.h>
#include <Rendering/Lights/ILight.h>
#include <Rendering/Lights/DirectionalLight.h>
#include <Rendering/Buffers/VertexConstantBuffer.h>
#include <Scene/Scene.h>
#include <Scene/SceneGraph/SceneNode.h>
#include <Systems/RenderSystem.h>
#include <Systems/TransformSystem.h>
#include <Systems/CameraSystem.h>
#include <Systems/PhysicsSystem.h>
#include "Systems/LightSystem.h"

Engine::Systems::RenderSystem::~RenderSystem()
{
    delete m_instance;
}



Engine::Rendering::Lights::ILight::LightData lights[4];


void Engine::Systems::RenderSystem::DrawScene(float p_deltaTime, bool p_isEditor)
{
    HRESULT hr;
    Rendering::Renderer::GetInstance()->Bind();
    Rendering::Renderer::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    auto camera = Systems::CameraSystem::GetCamera(GetInstance()->m_activeCamera);

    for (auto& sceneNode : Managers::SceneManager::GetActiveScene()->GetSceneGraph().GetAllSceneNodes())
    {
        if (auto model = sceneNode.second->GetGameObject()->FindComponentOfType<Components::ModelComponent>())
        {
            if (model->IsActive())
                DrawSceneNode(sceneNode.second);
        }
    }

    if (!Systems::LightSystem::GetAllLights().empty())
    {
        int j = 0;
        for (int i = 0; i < LightSystem::GetLights().size() && j < 4; ++i)
        {
            const std::shared_ptr<Components::Light> light1 = Systems::LightSystem::GetLights()[i];
            if (light1->GetGameObject()->IsActive())
            {
                std::shared_ptr<Rendering::Lights::ILight> lightType = light1->GetLight();
                lights[j] = lightType->GetLightData();
                ++j;
            }
        }
    }

    if (GetInstance()->m_debugMode)
    {
        for (auto collider : Systems::PhysicsSystem::GetColliders())
        {
            if (collider.second->IsActive())
            {
                auto model = collider.second->GetModel();
                auto mesh  = model->GetMeshes()[0];
                mesh->GenerateBuffers(Rendering::Renderer::GetInstance()->GetDevice());
                mesh->Bind(Rendering::Renderer::GetInstance()->GetContext());

                Matrix4F modelMatrix = collider.second->GetWorldMatrix();
                Matrix4F normalModel = Matrix4F::Inverse(modelMatrix);

                Matrix4F view        = camera->GetViewMatrix();
                Matrix4F perspective = camera->GetPerspectiveMatrix();

                Rendering::Buffers::VCB vcb{modelMatrix, view, normalModel, perspective};
                mesh->GetMaterial()->GetVertexShader()->GetVCB().Update(vcb);
                const Vector3F cameraPos = camera->GetPosition();

                // const Rendering::Buffers::PCB pcb{
                //     Vector4F::zero, Vector4F::one, Vector4F::one,
                //     Vector4F::zero, Vector4F::one,
                //     1.0f, Vector3F{}, Vector3F::zero,
                //     static_cast<float>(mesh->GetMaterial()->GetTextureState()), mesh->GetMaterial()->GetColor()
                // };

                //create empty lights
                // Rendering::Lights::DirectionalLight::LightData lights[4];
                const Rendering::Buffers::PCB pcb{
                    {lights[0], lights[1], lights[2], lights[3]}, GetInstance()->m_ambientColor,Vector3F::zero,
                        static_cast<float>(mesh->GetMaterial()->GetTextureState()), mesh->GetMaterial()->GetColor()
                };

                mesh->GetMaterial()->GetPixelShader()->GetPCB().Update(pcb);
                Rendering::Renderer::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

                Rendering::Renderer::GetInstance()->Bind(Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetTarget(),
                                                        Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetDepthStencilView());

                GFX_THROW_INFO_ONLY(Rendering::Renderer::GetInstance()->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));

                Rendering::Renderer::GetInstance()->Bind();

                Rendering::Renderer::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            }
        }

        Rendering::Renderer::GetInstance()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    }

    //Draw to ImGUI Image but not to screen rect
    if (p_isEditor)
    {
        static bool      open         = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | !ImGuiWindowFlags_HorizontalScrollbar |
                                        ImGuiWindowFlags_NoScrollWithMouse;
        ImGui::Begin("Scene", &open, window_flags);
        ImGui::SetScrollX(ImGui::GetScrollMaxX() * 0.5f);
        ImGui::SetScrollY(ImGui::GetScrollMaxY() * 0.5f);
        const Vector2F& rect = Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetRect();
        ImGui::Image(Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetShaderResourceView().Get(),
                     ImVec2(rect.x, rect.y));
        ImGui::End();
    }
    //Draw to Screen Rect
    else
    {
        std::shared_ptr<ObjectElements::Mesh> screenRect = Rendering::Renderer::GetInstance()->GetRect();
        screenRect->Bind(Rendering::Renderer::GetInstance()->GetContext());

        const Rendering::Buffers::VCB vcb{
            Matrix4F::identity, Matrix4F::identity,
            Matrix4F::identity, Matrix4F::identity
        };

        screenRect->GetMaterial()->GetVertexShader()->GetVCB().Update(vcb);
        screenRect->GetMaterial()->SetTextureState(true);

        // const Rendering::Buffers::PCB pcb{
        //     Vector4F::zero, Vector4F::one, Vector4F::one,
        //     Vector4F::zero, Vector4F::one,
        //     1.0f, Vector3F{}, Vector3F::zero,
        //     static_cast<float>(screenRect->GetMaterial()->GetTextureState()), screenRect->GetMaterial()->GetColor()
        // };

        //create empty lights
        // Rendering::Lights::DirectionalLight::LightData lights[4];
        const Rendering::Buffers::PCB pcb{
            {lights[0], lights[1], lights[2], lights[3]}, GetInstance()->m_ambientColor,Vector3F::zero,
                static_cast<float>(screenRect->GetMaterial()->GetTextureState()), screenRect->GetMaterial()->GetColor()
        };

        screenRect->GetMaterial()->GetPixelShader()->GetPCB().Update(pcb);

        screenRect->GetMaterial()->GetTexture()->SetTextureShaderResourceView(Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetShaderResourceView());

        Rendering::Renderer::GetInstance()->Bind();

        GFX_THROW_INFO_ONLY(Rendering::Renderer::GetInstance()->GetContext()->DrawIndexed(static_cast<UINT>(screenRect->GetIndices().size()), 0u, 0u));
        screenRect->Unbind(Rendering::Renderer::GetInstance()->GetContext());
    }
}

void Engine::Systems::RenderSystem::DrawSceneNode(std::shared_ptr<Scene::SceneNode> p_sceneNode)
{
    auto camera = GetActiveCamera();
    auto mesh   = p_sceneNode->GetMesh();



    if (p_sceneNode->IsRoot())
    {
        auto mat = p_sceneNode->GetGameObject()->FindComponentOfType<Components::ModelComponent>()->GetMaterial();
        if (mat != nullptr)
        {
            for (auto child : p_sceneNode->GetAllChildren())
            {
                child->GetMesh()->SetMaterial(mat);
            }
        }
    }

    if (mesh != nullptr)
    {
        mesh->Bind(Rendering::Renderer::GetInstance()->GetContext());

        Matrix4F model = TransformSystem::FindTransform(p_sceneNode->GetTransform())->GetWorldTransformMatrix();

        Matrix4F normalModel = Matrix4F::Inverse(model);

        Matrix4F view        = camera->GetViewMatrix();
        Matrix4F perspective = camera->GetPerspectiveMatrix();

        Rendering::Buffers::VCB vcb{model, view, normalModel, perspective};
        mesh->GetMaterial()->GetVertexShader()->GetVCB().Update(vcb);

        float txst = static_cast<float>(mesh->GetMaterial()->GetTextureState());
        // const Rendering::Buffers::PCB pcb{
        //     reversedXLightPos, light.ambient, light.diffuse,
        //     light.specular, light.color, light.shininess, Vector3F{},
        //     camera->GetPosition(), txst,
        //     mesh->GetMaterial()->GetColor()
        // };

        Vector3F invertedZCameraPos{ camera->GetPosition().x, camera->GetPosition().y, -camera->GetPosition().z };

        const Rendering::Buffers::PCB pcb{
            {lights[0], lights[1], lights[2], lights[3]}, GetInstance()->m_ambientColor,
            invertedZCameraPos, txst,
            mesh->GetMaterial()->GetColor()
        };

        mesh->GetMaterial()->GetPixelShader()->GetPCB().Update(pcb);

        Rendering::Renderer::GetInstance()->Bind(Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetTarget(),    
                                                 Rendering::Renderer::GetInstance()->GetRenderTextures()[0].GetDepthStencilView());

        GFX_THROW_INFO_ONLY(Rendering::Renderer::GetInstance()->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));

        Rendering::Renderer::GetInstance()->Bind();
    }

    for (auto child : p_sceneNode->GetChildren())
    {
        DrawSceneNode(child);
    }
}

void Engine::Systems::RenderSystem::IUpdate(float p_deltaTime, bool p_isEditor)
{
    Managers::SceneManager::GetActiveScene()->GetSceneGraph().UpdateScene(p_deltaTime);
    DrawScene(p_deltaTime, p_isEditor);
}

void Engine::Systems::RenderSystem::ResetActiveCamera()
{
    GetInstance()->m_activeCamera = -1;
}

void Engine::Systems::RenderSystem::SetActiveCamera(int32_t p_id)
{
    GetInstance()->m_activeCamera = p_id;
}

Engine::Systems::RenderSystem* Engine::Systems::RenderSystem::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new RenderSystem();
    }

    return m_instance;
}

std::shared_ptr<Engine::Components::Camera> Engine::Systems::RenderSystem::GetActiveCamera()
{
    return CameraSystem::GetCamera(GetInstance()->m_activeCamera);
}
