#include <stdafx.h>
#include <Rendering/Camera.h>
#include <Tools/ImGUI/imgui.h>
#include <Input/Input.h>
#include "Containers/EventContainer.h"


void Engine::Rendering::Camera::UpdateCamera(const float& p_width, const float& p_height)
{
    UpdateCameraPosition();
    UpdateVectors();
    UpdateCameraRotation();
    UpdateViewMatrix();
    UpdateResolution(p_width, p_height);
}

Engine::Rendering::Camera::Camera(const int32_t p_transformId, const int p_width, const int p_height) :
m_transformId(p_transformId), m_width(static_cast<float>(p_width)), m_height(static_cast<float>(p_height))
{
    UpdatePerspectiveMatrix();
}

void Engine::Rendering::Camera::UpdateVectors()
{
     //Supposedly ok.
     const Quaternion pitch = Quaternion(Vector3F(1.0f, 0.0f, 0.0f), GPM::Tools::Utils::ToRadians(m_pitch));
     const Quaternion yaw   = Quaternion(Vector3F(0.0f, 1.0f, 0.0f), GPM::Tools::Utils::ToRadians(m_yaw));
     const Quaternion roll  = Quaternion(Vector3F(0.0f, 0.0f, 1.0f), GPM::Tools::Utils::ToRadians(0.0f));
    
     auto transform = Containers::TransformContainer::GetTransform(m_transformId);

     transform->SetRotation((roll * yaw * pitch).Normalize());
     //const Matrix4F coucou = Matrix4F::CreateRotation((pitch * yaw * roll).Normalize().Conjugate()).Transpose();
     //transform->SetRotation(Quaternion(Matrix4F{ coucou }));
}

void Engine::Rendering::Camera::UpdateCameraPosition()
{
    auto transform = Containers::TransformContainer::GetTransform(m_transformId);

    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraX", &transform->GetPosition().x, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraY", &transform->GetPosition().y, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraZ", &transform->GetPosition().z, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("Camera FOV", &m_fovAngle, 10.f, 180.f, "%1.f");
    }ImGui::End();

    if (ImGui::Begin("DirectionInfo"))
    {
        ImGui::Text("Forward: %f | %f | %f", transform->GetForward().x, transform->GetForward().y, transform->GetForward().z);
        ImGui::Text("Up: %f | %f | %f", transform->GetUp().x, transform->GetUp().y, transform->GetUp().z);
        ImGui::Text("Right: %f | %f | %f", transform->GetRight().x, transform->GetRight().y, transform->GetRight().z);
    }ImGui::End();

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::Space))
        m_speed = 0.2f;
    else
        m_speed = 0.05f;

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::W))
    {
        transform->Translate(transform->GetForward() * m_speed);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::S))
    {
        transform->Translate(transform->GetForward() * m_speed * -1);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::A))
    {
        transform->Translate(transform->GetRight() * m_speed * -1);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::D))
    {
        transform->Translate(transform->GetRight() * m_speed);
    }

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::Q))
    {
        transform->Translate(transform->GetUp() * m_speed);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::E))
    {
        transform->Translate(transform->GetUp() * m_speed * -1);
    }

}

void Engine::Rendering::Camera::UpdateCameraRotation()
{
    const float sensitivity{ 0.1f };
    float xPos{ static_cast<float>(_INPUT->mouse.GetRawPosition()->x) };
    float yPos{ static_cast<float>(_INPUT->mouse.GetRawPosition()->y) };

    xPos *= sensitivity;
    yPos *= sensitivity;

     m_yaw += xPos;
     m_pitch += yPos;

     if (m_pitch > 180.0f)
         m_pitch -= 360.0f;
     else if(m_pitch < -180.0f)
         m_pitch += 360.0f;

     if (m_yaw > 180.0f)
         m_yaw -= 360.0f;
     if (m_yaw < -180.0f)
         m_yaw += 360.0f;

     if (ImGui::Begin("Camera Tool"))
     {
         ImGui::SliderFloat("CameraYaw", &m_yaw, -180.0f, 180.0f, "%.1f");
         ImGui::SliderFloat("CameraPitch", &m_pitch, -180.0f, 180.0f, "%.1f");
     }ImGui::End();
}

void Engine::Rendering::Camera::UpdateViewMatrix()
{
    auto transform = Containers::TransformContainer::GetTransform(m_transformId);
    const Matrix4F rotation = transform->GetRotation().Conjugate().ToMatrix4().Transpose();
    const Matrix4F translation = Matrix4F::CreateTranslation(transform->GetPosition());

    m_viewMatrix = rotation * translation;
}

void Engine::Rendering::Camera::UpdateResolution(const float p_width, const float p_height)
{
    m_width = p_width;
    m_height = p_height;
    UpdatePerspectiveMatrix();
}

void Engine::Rendering::Camera::UpdatePerspectiveMatrix() noexcept
{
    const float twoNearZ = m_nearZ + m_nearZ;
    const float fRange = m_farZ / (m_farZ - m_nearZ);

    const float radAngle = GPM::Tools::Utils::ToRadians(m_fovAngle);

    float yScale = static_cast<float>(GPM::Tools::Utils::Tan(radAngle / 2));
    yScale = 1 / yScale;

    const float aspectRatio = m_width / m_height;
    const float xScale = yScale / aspectRatio;

    

    m_perspectiveMatrix = Matrix4F{xScale, 0.0f, 0.0f, 0.0f,
                                        0.0f, yScale, 0.0f, 0.0f,
                                        0.0f, 0.0f, fRange, 1.0f,
                                        0.0f, 0.0f, -m_nearZ * fRange, 0.0f};
}