#include <stdafx.h>
#include <Systems/RenderSystem.h>
#include <Managers/ModelManager.h>
#include "Tools/DirectX/GraphicsMacros.h"
#include "Rendering/Light.h"
#include "Tools/ImGUI/imgui.h"
#include "Tools/ImGUI/imgui_impl_dx11.h"
#include <Managers/GameObjectManager.h>
#include "Managers/TransformManager.h"
#include "Components/ModelComponent.h"

Engine::Systems::RenderSystem::RenderSystem(Rendering::Renderer* p_renderer): m_renderer(p_renderer)
{
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

    
    for (auto& gameObject : Managers::GameObjectManager::GetAllGameObjects())
    {
        for (auto& component : gameObject.second->GetAllComponents())
        {
            if (Components::ModelComponent* modelComp = dynamic_cast<Components::ModelComponent*>(component))
            {
                auto& meshes = Managers::ModelManager::FindModel(modelComp->m_model)->GetMeshes();

                for (auto mesh : meshes)
                {
                    mesh->Bind(m_renderer->GetContext());

                    // ********* WIP ********* //

            Vector3D quat{ 0, 1, 0 };
                    // create constant buffer for transform matrix
                    struct VertexConstantBuffer
                    {
                        Matrix4F model;
                        Matrix4F view;
                        Matrix4F normalModel;
                        Matrix4F projection;
                    };

                    Matrix4F model = Managers::TransformManager::FindTransform(gameObject.second->GetTransformID())->GetTransformMatrix();

                    Matrix4F normalModel = Matrix4F::Inverse(model);

                    Matrix4F view = m_camera.GetViewMatrix();
                    Matrix4F perspective = m_camera.GetPerspectiveMatrix();

                    model.Transpose();
                    view.Transpose();
                    normalModel.Transpose();
                    perspective.Transpose();

                    const VertexConstantBuffer vcb{ model, view, normalModel,perspective };

                    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexConstantBuffer;
                    D3D11_BUFFER_DESC                    vertexBufferDesc = {};
                    vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
                    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    vertexBufferDesc.MiscFlags = 0u;
                    vertexBufferDesc.ByteWidth = sizeof(vcb);
                    vertexBufferDesc.StructureByteStride = 0u;
                    D3D11_SUBRESOURCE_DATA VertexConstantShaderData = {};
                    VertexConstantShaderData.pSysMem = &vcb;
                    GFX_THROW_INFO(m_renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &VertexConstantShaderData, &vertexConstantBuffer));

                    //bind the buffer to the shader
                    m_renderer->GetContext()->VSSetConstantBuffers(0u, 1u, vertexConstantBuffer.GetAddressOf());

                    struct PixelConstantBuffer
                    {
                        Rendering::Light lightSource;
                        Vector3F cameraPos;
                    };

                    const Vector4F invertedXLightPos{ -light->position.x, light->position.y, light->position.z, light->position.w };

                    const PixelConstantBuffer pcb{ invertedXLightPos,
                                                  light->ambient,
                                                  light->diffuse,
                                                  light->specular,
                                                  light->direction,
                                                  light->color,
                                                  64.0f,
                                                  m_camera.GetPosition() };

                    Microsoft::WRL::ComPtr<ID3D11Buffer> pixelConstantBuffer;
                    D3D11_BUFFER_DESC                    pixelBufferDesc = {};
                    pixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                    pixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
                    pixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                    pixelBufferDesc.MiscFlags = 0u;
                    pixelBufferDesc.ByteWidth = sizeof(pcb);
                    pixelBufferDesc.StructureByteStride = 0u;

                    D3D11_SUBRESOURCE_DATA PixelConstantShaderData = {};
                    PixelConstantShaderData.pSysMem = &pcb;
                    GFX_THROW_INFO(m_renderer->GetDevice()->CreateBuffer(&pixelBufferDesc, &PixelConstantShaderData, &pixelConstantBuffer));

                    //bind the buffer to the shader
                    m_renderer->GetContext()->PSSetConstantBuffers(0u, 1u, pixelConstantBuffer.GetAddressOf());

                    m_renderer->LoadPixelShader(L"../Engine/Resources/Shaders/PixelShader.cso");
                    m_renderer->LoadVertexShader(L"../Engine/Resources/Shaders/VertexShader.cso");

                    m_renderer->GetContext()->OMSetRenderTargets(1u, m_renderer->GetTarget().GetAddressOf(), m_renderer->GetDepthStencil().Get());

                    //set primitive draw
                    m_renderer->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    //create input layout
                    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
                    const D3D11_INPUT_ELEMENT_DESC            inputDesc[] =
                    {
                        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                        {"TxCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0},
                        {"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20u, D3D11_INPUT_PER_VERTEX_DATA, 0}
                    };
                    GFX_THROW_INFO(m_renderer->GetDevice()->CreateInputLayout(inputDesc,
                        std::size(inputDesc),
                        m_renderer->GetBlob()->GetBufferPointer(),
                        (UINT)m_renderer->GetBlob()->GetBufferSize(),
                        &inputLayout));

                    m_renderer->GetContext()->IASetInputLayout(inputLayout.Get());

                    ImGui_ImplDX11_Init(m_renderer->GetDevice().Get(), m_renderer->GetContext().Get());

                    GFX_THROW_INFO_ONLY(m_renderer->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));
                }
            }
        }
    }
}

void Engine::Systems::RenderSystem::Update()
{
    m_camera.UpdateCamera();
    DrawScene();
}

uint32_t Engine::Systems::RenderSystem::AddModel(const std::string& p_path, const std::string& p_name)
{
    if (Managers::ModelManager::FindModel(p_name) >= 0)
    {
        const std::string error("in Engine::Systems::RenderSystem::AddModel(const std::string& p_path, const std::string& p_name): Could not add model with name " +
                                p_name + " because it already exists");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return -1;
    }


    std::shared_ptr newModel = Managers::ModelManager::AddModel(p_path, p_name);

    if (newModel)
    {     
        const uint32_t tmpId = Tools::IDCounter::GetNewID();
        return tmpId;
    }
    else
        return -1;
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
