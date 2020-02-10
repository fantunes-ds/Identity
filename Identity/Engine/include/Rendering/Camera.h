#pragma once
#include <Export.h>
#include <GPM/GPM.h>
#include <Objects/IObject.h>

namespace Engine::Systems
{
    class RenderSystem;
}

namespace Engine::Rendering
{
    class API_ENGINE Camera : public Objects::IObject
    {
        friend class Systems::RenderSystem;
    public:
        Camera() = default;
        Camera(const int p_width, const int p_height);
        ~Camera() = default;

        void UpdateCamera();

        [[nodiscard]] Matrix4F GetPerspectiveMatrix() const noexcept;
        [[nodiscard]] const Matrix4F& GetViewMatrix() const noexcept { return m_viewMatrix; };

        [[nodiscard]] const Vector3F& GetPosition() const noexcept { return m_position; }
        [[nodiscard]] const Quaternion& GetOrientation() const noexcept { return m_orientation; }

        [[nodiscard]] const float& GetYaw() const noexcept { return m_yaw; }
        [[nodiscard]] const float& GetPitch() const noexcept { return m_pitch; }

        [[nodiscard]] const float& GetMovementSpeed() const noexcept { return m_speed; }
        [[nodiscard]] const float& GetMouseSensitivity() const noexcept { return m_sensitivity; }
        [[nodiscard]] const float& GetZoom() const noexcept { return m_zoom; }


        void SetPosition(const Vector3F& p_position) { m_position = p_position; }
        void SetYaw(const float& p_yaw) { m_yaw = p_yaw; }
        void SetPitch(const float& p_pitch) { m_pitch = p_pitch; }

    private:
        void UpdateViewMatrix();
        void UpdateResolution(const int p_width, const int p_height);
        void UpdateVectors();
        void UpdateCameraPosition();
        void UpdateCameraRotation();


        
        float m_speed{ 0.05f };
        float m_sensitivity{ 0.05f };

        // Perpsective variables
        float m_angle = 90.0f;
        float m_width{ 1.0f };
        float m_height{ 9.0f / 16.0f };
        float m_nearZ{ 0.5f };
        float m_farZ{ 1000.0f };

        float m_zoom{ 45.0f };
        float m_yaw{ 180.0f };
        float m_pitch{ 0.00f };
        float m_lastX{ m_width * 0.5f };
        float m_lastY{ m_height * 0.5f };

        Vector3F m_position{ 0.0f, 0.0f, -10.0f };
        Quaternion m_orientation{ 0.0f, 0.0f, 0.0f, -1.0f };

        Matrix4F m_viewMatrix;
    };
}
