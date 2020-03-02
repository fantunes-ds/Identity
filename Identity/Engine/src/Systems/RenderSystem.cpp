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

//WIP

//Example of how to use events
Engine::Systems::RenderSystem::RenderSystem()
{
    /*Containers::EventContainer::AddEvent("NoActiveCamera");
    Event& event = Containers::EventContainer::GetEvent("NoActiveCamera");
    event.AddListener(this, &RenderSystem::ResetActiveCamera);*/
}

void Engine::Systems::RenderSystem::DrawScene()
{
    HRESULT hr;

    std::shared_ptr<Rendering::Lights::Light>  light1 = std::dynamic_pointer_cast<Rendering::Lights::Light>(Containers::LightContainer::GetLights().begin()->second);
    Rendering::Lights::Light::LightData& light = light1->GetLightData();

    std::shared_ptr<Rendering::Camera> camera = Containers::CameraContainer::GetCamera(m_activeCamera);    

    D3D11_TEXTURE2D_DESC            textureDesc;
    D3D11_RENDER_TARGET_VIEW_DESC   renderTargetViewDesc;
    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

    /////////////////////////// Map's Texture
    //// Initialize the  texture description.
    ZeroMemory(&textureDesc, sizeof(textureDesc));

    // Setup the texture description.
    // We will have our map be a square
    // We will need to have this texture bound as a render target AND a shader resource
    textureDesc.Width            = Rendering::Renderer::GetInstance()->GetWidth();
    textureDesc.Height           = Rendering::Renderer::GetInstance()->GetHeight();
    textureDesc.MipLevels        = 1;
    textureDesc.ArraySize        = 1;
    textureDesc.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage            = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags   = 0;
    textureDesc.MiscFlags        = 0;

    // Create the texture
    Rendering::Renderer::GetInstance()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTextureMap);

    ///////////////////////// Map's Render Target
    //// Setup the description of the render target view.
    renderTargetViewDesc.Format             = textureDesc.Format;
    renderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    // Create the render target view.
    Rendering::Renderer::GetInstance()->GetDevice()->CreateRenderTargetView(*m_renderTargetTextureMap.GetAddressOf(), &renderTargetViewDesc, &m_renderTargetViewMap);
    /////////////////////// Map's Shader Resource View
    // Setup the description of the shader resource view.
    shaderResourceViewDesc.Format                    = textureDesc.Format;
    shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
    shaderResourceViewDesc.Texture2D.MipLevels       = 1;

    //// Create the shader resource view.
    Rendering::Renderer::GetInstance()->GetDevice()->CreateShaderResourceView(*m_renderTargetTextureMap.GetAddressOf(),
                                                                              &shaderResourceViewDesc,
                                                                              &m_shaderResourceViewMap);

    ////////////////////////// Draw Terrain Onto Map
    // Here we will draw our map, which is just the terrain from the mapCam's view

    


    const float colour[] = { 0.5f, 0.3f, 0.3f, 1.0f };
    Rendering::Renderer::GetInstance()->GetContext()->ClearRenderTargetView(*m_renderTargetViewMap.GetAddressOf(), colour);
    Rendering::Renderer::GetInstance()->GetContext()->ClearDepthStencilView(*Rendering::Renderer::GetInstance()->GetDepthStencilView().GetAddressOf(), D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.0f, 0);


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

                    // Set our maps Render Target
                    Rendering::Renderer::GetInstance()->SetRenderTarget(m_renderTargetViewMap);
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
    ImGuiWindowFlags window_flags = !ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollbar;
    ImGui::Begin("Scene", &open, window_flags);
    ImGui::SetScrollX(ImGui::GetScrollMaxX() * 0.5f);
    ImGui::SetScrollY(ImGui::GetScrollMaxY() * 0.5f);
    ImGui::Image(m_shaderResourceViewMap.Get(), ImVec2(textureDesc.Width,
                                                                       textureDesc.Height));
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
