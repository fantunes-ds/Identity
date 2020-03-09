#pragma once
#include <Export.h>
#include <GPM/GPM.h>
#include <Components/IComponent.h>
#include <Objects/GameObject.h>
// #include <Containers/CameraSystem.h>

namespace Engine::Components
{
    class API_ENGINE Camera : public IComponent
    {

    public:
        Camera() = default;
        Camera(Objects::GameObject* p_gameObject, const int p_width, const int p_height);
        ~Camera() = default;

        bool operator==(IComponent* p_other) override;
        bool DeleteFromMemory() override;

        //Will be called by system if needed
        void UpdateCamera(const float p_deltaTime, const float& p_width, const float& p_height);
        //------

        [[nodiscard]] const Matrix4F& GetPerspectiveMatrix() const noexcept { return m_perspectiveMatrix; }
        [[nodiscard]] const Matrix4F& GetViewMatrix() const { return m_viewMatrix; }

        [[nodiscard]] const Vector3F& GetPosition() const noexcept { return m_gameObject->GetTransform()->GetPosition(); }
        [[nodiscard]] const Quaternion& GetRotation() const noexcept { return m_gameObject->GetTransform()->GetRotation(); }

        [[nodiscard]] const float& GetMovementSpeed() const noexcept { return m_speed; }
        [[nodiscard]] const float& GetMouseSensitivity() const noexcept { return m_sensitivity; }
        [[nodiscard]] const float& GetZoom() const noexcept { return m_zoom; }

    private:
        void UpdateViewMatrix();
        void UpdateResolution(const float p_width, const float p_height);
        void UpdatePerspectiveMatrix() noexcept;
        void UpdateVectors();
        void UpdateCameraPosition(const float p_deltaTime);
        void UpdateCameraRotation();

        bool needUpdate{ false };

        float m_speed{ 0.05f };
        float m_sensitivity{ 0.05f };

        // Perpsective variables
        float m_fovAngle{ 60.0f };
        float m_width{ 1.0f };
        float m_height{ 9.0f / 16.0f };
        float m_nearZ{ 0.1f };
        float m_farZ{ 1000.0f };

        float m_zoom{ 45.0f };
        float m_yaw{ 0.0f };
        float m_pitch{ 0.0f };
        float m_lastX{ m_width * 0.5f };
        float m_lastY{ m_height * 0.5f };

        Matrix4F m_viewMatrix;
        Matrix4F m_perspectiveMatrix;
    };
}
