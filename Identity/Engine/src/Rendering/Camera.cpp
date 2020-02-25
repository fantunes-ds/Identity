#include <stdafx.h>
#include <Rendering/Camera.h>
#include <Tools/ImGUI/imgui.h>
#include <Input/Input.h>
#include "Containers/EventContainer.h"


void Engine::Rendering::Camera::UpdateCamera(const float& p_width, const float& p_height)
{
    UpdateCameraPosition();
    UpdateCameraRotation();
    UpdateVectors();
    UpdateViewMatrix();
    UpdateResolution(p_width, p_height);
}

Engine::Rendering::Camera::Camera(const int p_width, const int p_height) : m_width(static_cast<float>(p_width)), m_height(static_cast<float>(p_height))
{
    UpdatePerspectiveMatrix();
}

void Engine::Rendering::Camera::UpdateVectors()
{
    const Quaternion pitch = Quaternion(Vector3F(-1.0f, 0.0f, 0.0f), GPM::Tools::Utils::ToRadians(m_pitch));
    const Quaternion yaw   = Quaternion(Vector3F(0.0f, 1.0f, 0.0f), GPM::Tools::Utils::ToRadians(-m_yaw));
    const Quaternion roll  = Quaternion(Vector3F(0.0f, 0.0f, 1.0f), GPM::Tools::Utils::ToRadians(0.0f));

    m_orientation = pitch * yaw * roll;
    m_orientation = m_orientation.Normalize();
}

void Engine::Rendering::Camera::UpdateCameraPosition()
{
    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraX", &m_position.x, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraY", &m_position.y, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraZ", &m_position.z, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("Camera FOV", &m_angle, 10.f, 180.f, "%1.f");
    }ImGui::End();

    const Quaternion forward = m_orientation * Vector3F::forward * m_orientation.Conjugate();
    const Vector3F forwardAxis = forward.GetRotationAxis() * - 1;
    const Vector3 rightAxis = Vector3F::Cross(forwardAxis, Vector3D::up) * -1;

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::Space))
        m_speed = 0.2f;
    else
        m_speed = 0.05f;

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::W))
        m_position += forwardAxis.Normalized() * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::S))
        m_position -= forwardAxis.Normalized() * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::A))
        m_position -= rightAxis.Normalized()* m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::D))
        m_position += rightAxis.Normalized()* m_speed;
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

    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraYaw", &m_yaw, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("CameraPitch", &m_pitch, -180.0f, 180.0f, "%.1f");
    }ImGui::End();
}


void Engine::Rendering::Camera::UpdateViewMatrix()
{
    const Quaternion reversedOrientation = m_orientation.Conjugate();
    const Matrix4F rotation = reversedOrientation.ToMatrix4();

    const Matrix4F translation = Matrix4F::CreateTranslation(Vector3F(m_position.x, m_position.y, m_position.z)).Transpose();
    m_viewMatrix = translation * rotation;
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

    const float radAngle = GPM::Tools::Utils::ToRadians(m_angle);

    float yScale = static_cast<float>(GPM::Tools::Utils::Tan(radAngle / 2));
    yScale = 1 / yScale;

    const float aspectRatio = m_width / m_height;
    const float xScale = yScale / aspectRatio;

    

    m_perspectiveMatrix = Matrix4F{xScale, 0.0f, 0.0f, 0.0f,
                                        0.0f, yScale, 0.0f, 0.0f,
                                        0.0f, 0.0f, fRange, 1.0f,
                                        0.0f, 0.0f, -m_nearZ * fRange, 0.0f};
}