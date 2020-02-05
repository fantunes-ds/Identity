#include <stdafx.h>
#include <Rendering/Camera.h>
#include <Tools/ImGUI/imgui.h>

Engine::Rendering::Camera::Camera(const int p_width, const int p_height) : m_width(p_width), m_height(m_height)
{
}

void Engine::Rendering::Camera::UpdateVectors()
{
    if (ImGui::Begin("Camera Tool"))
    {
        ImGui::SliderFloat("CameraX", &m_position.x, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraY", &m_position.y, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("CameraZ", &m_position.z, -10.0f, 10.0f, "%.1f");
        ImGui::SliderFloat("Camera FOV", &angle, 10.f, 180.f, "%1.f");
    }ImGui::End();

    m_direction = Vector3F(m_position - m_target).Normalized();
    const Vector3F worldUp = Vector3F(0.0f, 1.0f, 0.0f);
    m_right = Vector3F(Vector3F::Cross(worldUp, m_forward * -1));
    m_up = Vector3F::Cross(m_forward * - 1, m_right);
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

    float radAngle = GPM::Tools::Utils::ToRadians(angle);

    float yScale = GPM::Tools::Utils::Tan(radAngle / 2);
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
    return { Matrix4F::LookAt(m_position,
                              m_position + m_forward,
                              m_up) };
}
