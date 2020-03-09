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
     const Quaternion pitch = Quaternion(Vector3F(-1.0f, 0.0f, 0.0f), GPM::Tools::Utils::ToRadians(m_pitch));
     const Quaternion yaw   = Quaternion(Vector3F(0.0f, 1.0f, 0.0f), GPM::Tools::Utils::ToRadians(-m_yaw));
     const Quaternion roll  = Quaternion(Vector3F(0.0f, 0.0f, 1.0f), GPM::Tools::Utils::ToRadians(0.0f));
    
     auto transform = Containers::TransformContainer::GetTransform(m_transformId);

     transform->SetRotation((roll * yaw * pitch).Normalize());
     //const Matrix4F coucou = Matrix4F::CreateRotation((pitch * yaw * roll).Normalize().Conjugate()).Transpose();
     //transform->SetRotation(Quaternion(Matrix4F{ coucou }));
}

void Engine::Rendering::Camera::UpdateCameraPosition()
{
    auto transform = Containers::TransformContainer::GetTransform(m_transformId);

    float *pos [3] = { &transform->GetPosition().x, &transform->GetPosition().y, &transform->GetPosition().z };
    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::DragFloat3("CameraPosition", *pos, 0.1f, -10.0f, 10.0f, "%0.1f");
        ImGui::SliderFloat("Camera FOV", &m_fovAngle, 10.f, 180.f, "%1.f");
    }ImGui::End();

    if (ImGui::Begin("Camera DirectionInfo"))
    {
        ImGui::Text("Forward: %f | %f | %f", transform->GetForward().x, transform->GetForward().y, transform->GetForward().z);
        ImGui::Text("Up: %f | %f | %f", transform->GetUp().x, transform->GetUp().y, transform->GetUp().z);
        ImGui::Text("Right: %f | %f | %f", transform->GetRight().x, transform->GetRight().y, transform->GetRight().z);
    }ImGui::End();

    float speed = m_speed;

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::Space))
        speed *= 4.0f;

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::W))
    {
        transform->Translate(transform->GetForward() * speed);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::S))
    {
        transform->Translate(transform->GetForward() * speed * -1);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::D))
    {
        transform->Translate(transform->GetRight() * speed );
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::A))
    {
        transform->Translate(transform->GetRight() * speed * -1);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::E))
    {
        transform->Translate(transform->GetUp() * speed);
    }
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::Q))
    {
        transform->Translate(transform->GetUp() * speed * -1);
    }
}

void Engine::Rendering::Camera::UpdateCameraRotation()
{
    const float sensitivity{ 0.3f };
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
         ImGui::DragFloat("CameraYaw", &m_yaw, 1.f ,-180.0f, 180.0f, "%.1f");
         ImGui::DragFloat("CameraPitch", &m_pitch, 1.f,-180.0f, 180.0f, "%.1f");
     }ImGui::End();
}

void Engine::Rendering::Camera::UpdateViewMatrix()
{

    auto transform = Containers::TransformContainer::GetTransform(m_transformId);
    std::string gopos = "go x : " + std::to_string(transform->GetPosition().x) + "y : " + std::to_string(transform->GetPosition().y) + "z : " + std::to_string(transform->GetPosition().z + '\n');
    OutputDebugString(gopos.c_str());
    const Matrix4F rotation = transform->GetRotation().Conjugate().ToMatrix4().Transpose();
    const Matrix4F translation = Matrix4F::CreateTranslation(Vector3F{ -transform->GetPosition().x, -transform->GetPosition().y, transform->GetPosition().z});

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
    const float fRange = m_farZ / (m_nearZ - m_farZ);

    const float radAngle = GPM::Tools::Utils::ToRadians(m_fovAngle);

    float yScale = static_cast<float>(GPM::Tools::Utils::Tan(radAngle / 2));
    yScale = 1 / yScale;

    const float aspectRatio = m_width / m_height;
    const float xScale = yScale / aspectRatio;

    

    m_perspectiveMatrix = Matrix4F{xScale, 0.0f, 0.0f, 0.0f,
                                        0.0f, yScale, 0.0f, 0.0f,
                                        0.0f, 0.0f, fRange, m_nearZ* fRange,
                                        0.0f, 0.0f, -1.0f , 0.0f};
}
