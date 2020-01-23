#pragma once
#include <Export.h>
#include <GPM/GPM.h>

namespace Engine::ObjectElements
{
    class API_ENGINE Transform
    {
    public:
        Transform();

        Transform(GPM::Vector3D& p_position, GPM::Vector3D& p_scale, GPM::Vector3D& p_rotation);
        ~Transform() = default;

        Transform(const Transform& p_other);
        Transform(const Transform&& p_other) noexcept;

        /* @brief Adds the given vector to the current position.
         *
         */
        void Translate(const GPM::Vector3D& p_vector);

        /* @brief Rotates transform by a vector of Euler angles.
         * @warning: may contain error at m_rotation *= quat
         */
        void RotateWithEulerAngles(const GPM::Vector3D& p_euler);

        /* @brief Multiplies the Transform's current scale by the given vector.
         * 
         */
        void Scale(const GPM::Vector3D& p_scale);


        inline void SetPosition(const GPM::Vector3D& p_position) { m_position = p_position; }
        inline void SetScale(const GPM::Vector3D& p_scale) { m_scale = p_scale; }
        inline void SetRotation(const GPM::Quaternion& p_rotation) { m_rotation = p_rotation; }
        inline void SetParent(Transform* p_parent) { m_parent = p_parent; }

        [[nodiscard]] GPM::Vector3D GetEuler() const;
        inline GPM::Vector3D& GetPosition() { return m_position; }
        inline GPM::Vector3D& GetForward() { return m_forward; }
        inline GPM::Vector3D& GetRight() { return m_right; }
        inline GPM::Vector3D& GetScale() { return m_scale; }
        inline GPM::Quaternion& GetRotation() { return m_rotation; }
        inline Transform* GetParent() const { return m_parent; }

        inline void bullshit()
        {
            m_forward.x += 50;
        }

    private:
        GPM::Vector3D m_position;
        GPM::Vector3D m_forward;
        GPM::Vector3D m_right;
        GPM::Vector3D m_up;
        GPM::Vector3D m_scale;
        GPM::Quaternion m_rotation;
        Transform* m_parent = nullptr;
    };
}
