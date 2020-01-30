#pragma once
#include <Export.h>
#include <GPM/GPM.h>

namespace Engine::Rendering
{
    class API_ENGINE Camera
    {
        friend class Renderer;
    public:
        Camera() = default;
        ~Camera() = default;

        void UpdateVectors();

        [[nodiscard]] Matrix4F GetPerspectiveMatrix() const noexcept;
        [[nodiscard]] Matrix4F GetViewMatrix() const noexcept;

        [[nodiscard]] const Vector3F& GetPosition() const noexcept { return m_position; }
        [[nodiscard]] const Vector3F& GetFront() const noexcept { return m_forward; }
        [[nodiscard]] const Vector3F& GetUp() const noexcept { return m_up; }
        [[nodiscard]] const Vector3F& GetRight() const noexcept { return m_right; }

        [[nodiscard]] const float& GetYaw() const noexcept { return m_yaw; }
        [[nodiscard]] const float& GetPitch() const noexcept { return m_pitch; }

        [[nodiscard]] const float& GetMovementSpeed() const noexcept { return m_speed; }
        [[nodiscard]] const float& GetMouseSensitivity() const noexcept { return m_sensitivity; }
        [[nodiscard]] const float& GetZoom() const noexcept { return m_zoom; }

        void SetPosition(const Vector3F& p_position) { m_position = p_position; }
        void SetYaw(const float& p_yaw) { m_yaw = p_yaw; }
        void SetPitch(const float& p_pitch) { m_pitch = p_pitch; }

    private:
        Vector3F m_position{ 0.0f, 0.0f, -10.0f };
        Vector3F m_target;
        Vector3F m_direction;

        Vector3F m_right;
        Vector3F m_up;
        Vector3F m_forward{ 0.0f, 0.0f, -1.0f };

        float m_speed{ 0.05f };
        float m_sensitivity{ 0.05f };

        float m_zoom{ 45.0f };
        float m_yaw{ -90.0f };
        float m_pitch{ 0.00f };

        // Perpsective variables
        float m_width{ 1.0f };
        float m_height{ 3.0f / 4.0f };
        float m_nearZ{ 0.5f };
        float m_farZ{ 1000.0f };
    };
}
