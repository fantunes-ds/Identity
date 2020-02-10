#pragma once
#include <Export.h>
#include <GPM/GPM.h>
#include <Objects/IObject.h>

namespace Engine::ObjectElements
{
    class API_ENGINE Transform : public Objects::IObject
    {
    public:
        Transform();

        Transform(Vector3D& p_position);
        ~Transform() = default;

        Transform(const Transform& p_other);
        Transform(const Transform&& p_other) noexcept;

        /** 
         * @brief Adds the given vector to the current position.
         */
        void Translate(const Vector3D& p_vector);

        /**
         *@brief Rotates transform by a vector of Euler angles.
         *@warning: may contain error at m_rotation *= quat
         */
        void RotateWithEulerAngles(const Vector3D& p_euler);

        /**
         *@brief Multiplies the Transform's current scale by the given vector.
          
         */
        void Scale(const Vector3D& p_scale);


        inline void SetPosition(const Vector3D& p_position) { m_position = p_position; }
        inline void SetScale(const Vector3D& p_scale) { m_scale = p_scale; }
        inline void SetRotation(const Quaternion& p_rotation)
        {
            m_rotation = p_rotation;
            CalculateAxes();
        }

        inline void SetTransformMatrix(const Matrix4F& p_transform) { m_transform = p_transform; }
        void UpdateTransformMatrix();

        inline void SetParent(Transform& p_parent) { m_parent = std::make_shared<Transform>(p_parent); }

        [[nodiscard]] Vector3D GetEuler() const;
        inline Vector3D& GetPosition() { return m_position; }
        inline Vector3D& GetForward() { return m_forward; }
        inline Vector3D& GetRight() { return m_right; }
        inline Vector3D& GetUp() { return m_up; }
        inline Vector3D& GetScale() { return m_scale; }
        inline Quaternion& GetRotation() { return m_rotation; }
        inline std::shared_ptr<Transform> GetParent() const { return m_parent; }
        inline Matrix4F& GetTransformMatrix() { return m_transform; }

    private:
        void CalculateAxes();

        std::shared_ptr<Transform> m_parent = nullptr;
        Vector3D m_position;
        Vector3D m_forward;
        Vector3D m_right;
        Vector3D m_up;
        Vector3D m_scale;
        Quaternion m_rotation;

        Matrix4F m_transform;
    };
}
