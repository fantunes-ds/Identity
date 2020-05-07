#pragma once
#include <Export.h>
#include <GPM/GPM.h>
#include <Components/IComponent.h>
#include <Objects/GameObject.h>

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
        void Serialize(std::ostream& p_stream) override;
        void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) override;

        void SetActive(bool p_active) override { m_isActive = p_active; }

        /**
         * @brief Updates all variables related to the camera position and rotation if necessary.
         * This function is called by the CameraSystem.
         */
        void UpdateCamera(const float p_deltaTime, const float& p_width, const float& p_height);

        [[nodiscard]] const Matrix4F& GetPerspectiveMatrix() const noexcept { return m_perspectiveMatrix; }
        [[nodiscard]] const Matrix4F& GetViewMatrix() const { return m_viewMatrix; }

        [[nodiscard]] const Vector3F&   GetPosition() const noexcept { return m_gameObject->GetTransform()->GetPosition(); }
        [[nodiscard]] const Quaternion& GetRotation() const noexcept { return m_gameObject->GetTransform()->GetRotation(); }

        [[nodiscard]] const float& GetMovementSpeed() const noexcept { return m_speed; }
        [[nodiscard]] const float& GetMouseSensitivity() const noexcept { return m_sensitivity; }
        [[nodiscard]] const float& GetZoom() const noexcept { return m_zoom; }

    private:
        /**
         * @brief Updates the View (Camera) matrix. It is used to display objets from the camera's POV.
         * It's the second part of the MVP(Model, View, Perspective) matrix.
         */
        void UpdateViewMatrix();
        /**
         * @brief Updates the View (Camera) matrix. It is used to display objets from the camera's POV.
         * @param p_width : the new width of the camera.
         * @param p_height : the new height of the camera.
         */
        void UpdateResolution(const float p_width, const float p_height);
        /**
         * @brief Updates the resolution (width and height) of the camera so it displays correctly.
         */
        void UpdatePerspectiveMatrix() noexcept;
        /**
         * @brief Updates the position of the camera
         * @param p_deltaTime : The time in milliseconds
         */
        void UpdateCameraPosition(const float p_deltaTime);
        /**
         * @brief Updates the input for the orientation and that are going to be use on UpdateVectors().
         */
        void UpdateCameraRotation();
        /**
         * @brief Updates the Vectors for the orientation of the camera.
         */
        void UpdateVectors();

#pragma region GeneralVariables
        bool needUpdate{false};
        float m_speed{5.f};
        float m_sensitivity{0.05f};

        float m_zoom{45.0f};
        float m_yaw{0.0f};
        float m_pitch{0.0f};
        float m_lastX{m_width * 0.5f};
        float m_lastY{m_height * 0.5f};
#pragma endregion 

#pragma region Perspective Variables
        float m_fovAngle{60.0f};
        float m_width{1.0f};
        float m_height{9.0f / 16.0f};
        float m_nearZ{0.1f};
        float m_farZ{1000.0f};
#pragma endregion
#pragma region Matrices
        Matrix4F m_perspectiveMatrix;
        Matrix4F m_viewMatrix;
#pragma endregion
    };
}
