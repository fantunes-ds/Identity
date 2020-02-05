#include <stdafx.h>
#include <Systems/RenderSystem.h>
#include <Managers/ModelManager.h>
#include "Tools/DirectX/GraphicsMacros.h"
#include "Rendering/Light.h"
#include "Tools/ImGUI/imgui.h"
#include "Tools/ImGUI/imgui_impl_dx11.h"
#include <Input/Input.h>

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
        ImGui::SliderFloat("LightColX", &light->color.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("LightColY", &light->color.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("LightColZ", &light->color.z, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightX", &light->ambient.x, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightY", &light->ambient.y, 0.0f, 1.0f, "%.1f");
        ImGui::SliderFloat("Ambient LightZ", &light->ambient.z, 0.0f, 1.0f, "%.1f");
    }ImGui::End();

    for (auto& model : m_models)
    {
        for (auto& mesh : model.second->GetMeshes())
        {
            mesh->Bind(m_renderer->GetContext());
            m_renderer->SetRenderTarget();

            //------------SHADERS----------------
            //----------VERTEX CONSTANT----------
            struct VertexConstantBuffer
            {
                Matrix4F model;
                Matrix4F view;
                Matrix4F normalModel;
                Matrix4F projection;
            };

            Matrix4F normalModel = Matrix4F::Inverse(mesh->GetTransform());

            //----------INPUT------------
            if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::W))
                m_camera.m_position += m_camera.m_forward * m_camera.m_speed;
            if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::S))
                m_camera.m_position -= m_camera.m_forward * m_camera.m_speed;
            if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::A))
                m_camera.m_position -= Vector3F::Cross(m_camera.m_forward, m_camera.m_up).Normalized() * m_camera.m_speed;
            if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::D))
                m_camera.m_position += Vector3F::Cross(m_camera.m_forward, m_camera.m_up).Normalized() * m_camera.m_speed;
            //---------------------------

            Matrix4F view = m_camera.GetViewMatrix();
            Matrix4F perspective = m_camera.GetPerspectiveMatrix();

            Matrix4F Tmodel = mesh->GetTransform().Transpose();
            view.Transpose();
            normalModel.Transpose();
            perspective.Transpose();

            const VertexConstantBuffer vcb{ Tmodel, view, normalModel,perspective };

            Microsoft::WRL::ComPtr<ID3D11Buffer> vertexConstantBuffer;
            D3D11_BUFFER_DESC                    vertexBufferDesc = {};
            vertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;        //Dynamic - values can change
            vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            vertexBufferDesc.MiscFlags = 0u;
            vertexBufferDesc.ByteWidth = sizeof(VertexConstantBuffer);
            vertexBufferDesc.StructureByteStride = 0u;

            D3D11_SUBRESOURCE_DATA VertexConstantShaderData = {};
            VertexConstantShaderData.pSysMem = &vcb;
            GFX_THROW_INFO(m_renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &VertexConstantShaderData, &vertexConstantBuffer));

            //bind the buffer to the shader
            m_renderer->GetContext()->VSSetConstantBuffers(0u, 1u, vertexConstantBuffer.GetAddressOf());
            //--------------------------------

            //--------PIXEL CONSTANT----------
            struct PixelConstantBuffer
            {
                Rendering::Light lightSource;
                Vector3F cameraPos;
            };

            const PixelConstantBuffer pcb{ light->position,
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
            //---------------------------------------
            //---------------------------------------

            // ImGui_ImplDX11_Init(m_renderer->GetDevice().Get(), m_renderer->GetContext().Get());

            GFX_THROW_INFO_ONLY(m_renderer->GetContext()->DrawIndexed(static_cast<UINT>(mesh->GetIndices().size()), 0u, 0u));
        }
    }
}

void Engine::Systems::RenderSystem::Update()
{
    m_camera.UpdateVectors();
    DrawScene();
}


void Engine::Systems::RenderSystem::UpdateCamera()
{
    int width, height;
    m_renderer->GetResolution(width, height);
    m_camera.UpdateResolution(width, height);
}

uint32_t Engine::Systems::RenderSystem::AddModel(const std::string& p_path, const std::string& p_name)
{
    if (Manager::ModelManager::GetInstance()->FindModel(p_name))
    {
        const std::string error("in Engine::Systems::RenderSystem::AddModel(const std::string& p_path, const std::string& p_name): Could not add model with name " +
                                p_name + " because it already exists");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return -1;
    }


    std::shared_ptr newModel = Manager::ModelManager::GetInstance()->AddModel(p_path, p_name);

    if (newModel)
    {
        
        const uint32_t tmpId = Tools::IDCounter::GetID();
        m_models.insert_or_assign(tmpId, newModel);
        return tmpId;
    }
    else
        return -1;
}

uint32_t Engine::Systems::RenderSystem::AddLight(Rendering::Light& p_light)
{
    std::shared_ptr newLight = std::make_shared<Rendering::Light>(p_light);

    if (newLight)
    {
        const uint32_t tmpId = Tools::IDCounter::GetID();
        m_lights.insert_or_assign(tmpId, newLight);
        return tmpId;
    }
    else
        return -1;
}
