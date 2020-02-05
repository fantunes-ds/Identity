#include <stdafx.h>
#include <Rendering/Camera.h>
#include <Tools/ImGUI/imgui.h>
#include <Input/Input.h>


void Engine::Rendering::Camera::UpdateCamera()
{
    UpdateVectors();
    UpdateCameraPosition();
    UpdateCameraRotation();
}

Engine::Rendering::Camera::Camera(const int p_width, const int p_height) : m_width(p_width), m_height(m_height)
{
}

void Engine::Rendering::Camera::UpdateVectors()
{
    const Quaternion yaw = Quaternion(Vector3F(0.0f, 1.0f, 0.0f), Tools::Utils::ToRadians(-m_yaw));
    const Quaternion pitch = Quaternion(Vector3F(1.0f, 0.0f, 0.0f), Tools::Utils::ToRadians(m_pitch));
    m_orientation = yaw * pitch;
}

void Engine::Rendering::Camera::UpdateCameraPosition()
{
    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraX", &m_position.x, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraY", &m_position.y, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraZ", &m_position.z, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("Camera FOV", &angle, 10.f, 180.f, "%1.f");
    }ImGui::End();

    const Quaternion quaternionForward = m_orientation * Quaternion(m_position, 0.0f) * Quaternion::Conjugate(m_orientation);
    const Vector3F forward{ quaternionForward.GetRotationAxis() };
    const Vector3F right{ Vector3F::Cross(forward, Vector3F(0.0f, 1.0f, 0.0f)).Normalized() };

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::W))
        m_position += forward * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::S))
        m_position -= forward * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::A))
        m_position -= right * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::D))
        m_position += right * m_speed;
}

void Engine::Rendering::Camera::UpdateCameraRotation()
{
    float xPos{ static_cast<float>(_INPUT->mouse.GetRawDelta()->x) };
    float yPos{ static_cast<float>(_INPUT->mouse.GetRawDelta()->y) };

    const float sensitivity{ 0.1f };
    xPos *= sensitivity;
    yPos *= sensitivity;

    m_yaw += xPos;
    m_pitch += yPos;

    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraYaw", &m_yaw, -180.0f, 180.0f, "%.1f");
        ImGui::SliderFloat("CameraPitch", &m_pitch, -180.0f, 180.0f, "%.1f");
    }ImGui::End();

    UpdateVectors();
}


void Engine::Rendering::Camera::UpdateResolution(const int p_width, const int p_height)
{
    m_width = p_width;
    m_height = p_height;
}

Matrix4F Engine::Rendering::Camera::GetPerspectiveMatrix() const noexcept
{
    //Create perspective matrix

    const float twoNearZ = m_nearZ + m_nearZ;
    const float fRange = m_farZ / (m_farZ - m_nearZ);

    float radAngle = Tools::Utils::ToRadians(angle);

    float yScale = Tools::Utils::Tan(radAngle / 2);
    yScale = 1 / yScale;

    float AspectRatio = m_width / m_height;
    float xScale = yScale / AspectRatio;

    return {
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, fRange, 1.0f,
        0.0f, 0.0f, -m_nearZ * fRange, 0.0f
    };
}

Matrix4F Engine::Rendering::Camera::GetViewMatrix() const noexcept
{
    const Quaternion reversedOrientation = Quaternion::Conjugate(m_orientation);
    Matrix4F rotation = reversedOrientation.ToMatrix4();

    rotation.m_data[12] = (rotation.m_data[0] * m_position.x + rotation.m_data[4] * m_position.y + rotation.m_data[8] * m_position.z);
    rotation.m_data[13] = -(rotation.m_data[1] * m_position.x + rotation.m_data[5] * m_position.y + rotation.m_data[9] * m_position.z);
    rotation.m_data[14] = (rotation.m_data[2] * m_position.x + rotation.m_data[6] * m_position.y + rotation.m_data[10] * m_position.z);
    rotation.m_data[15] = 1;

    return rotation;
}