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

void Engine::Rendering::Camera::UpdateVectors()
{
    m_direction = Vector3F(m_position - m_target).Normalized();
    const Vector3F worldUp = Vector3F(0.0f, 1.0f, 0.0f);
    m_right = Vector3F(Vector3F::Cross(worldUp, m_direction * -1));
    m_up = Vector3F::Cross(m_direction * -1, m_right);
}

void Engine::Rendering::Camera::UpdateCameraPosition()
{
    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraX", &m_position.x, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraY", &m_position.y, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraZ", &m_position.z, -10.0f, 10.0f, "%.1f");
    }ImGui::End();

    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::W))
        m_position += m_forward * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::S))
        m_position -= m_forward * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::A))
        m_position += Vector3F::Cross(m_forward, m_up).Normalized() * m_speed;
    if (_INPUT->keyboard.IsKeyHeld(Input::Keyboard::D))
        m_position -= Vector3F::Cross(m_forward, m_up).Normalized() * m_speed;
}

void Engine::Rendering::Camera::UpdateCameraRotation()
{
    const float xPos { static_cast<float>(_INPUT->mouse.GetPosX())};
    const float yPos { static_cast<float>(_INPUT->mouse.GetPosY())};

    if (m_firstMouse) // initially set to true
    {
        m_lastX = xPos;
        m_lastY = yPos;
        m_firstMouse = false;
    }

    float xOffset{ xPos - m_lastX };
    float yOffset{ m_lastY - yPos }; // reversed since y-coordinates range from bottom to top
    m_lastX = xPos;
    m_lastY = yPos;

    const float sensitivity{ 0.05f };
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    Vector3F direction;
    direction.x = cos(Tools::Utils::ToRadians(m_yaw)) * cos(Tools::Utils::ToRadians(m_pitch));
    direction.y = sin(Tools::Utils::ToRadians(m_pitch));
    direction.z = sin(Tools::Utils::ToRadians(m_yaw)) * cos(Tools::Utils::ToRadians(m_pitch));
    m_forward = direction.Normalized();
}

Matrix4F Engine::Rendering::Camera::GetPerspectiveMatrix() const noexcept
{
    //Create perspective matrix

    const float twoNearZ = m_nearZ + m_nearZ;
    const float fRange = m_farZ / (m_farZ - m_nearZ);

    return {
        twoNearZ / m_width, 0.0f, 0.0f, 0.0f,
        0.0f, twoNearZ / m_height, 0.0f, 0.0f,
        0.0f, 0.0f, fRange, 1.0f,
        0.0f, 0.0f, -fRange * m_nearZ, 0.0f
    };
}

Matrix4F Engine::Rendering::Camera::GetViewMatrix() const noexcept
{
    const Vector3F invertedXCamPos { -m_position.x, m_position.y, m_position.z};
    return { Matrix4F::LookAt(invertedXCamPos,
                              invertedXCamPos + m_forward,
                              m_up) };
}
