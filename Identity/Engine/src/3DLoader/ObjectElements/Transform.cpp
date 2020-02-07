#include <stdafx.h>
#include <3DLoader/ObjectElements/Transform.h>

Engine::ObjectElements::Transform::Transform() :
    m_scale{ GPM::Vector3D::one }, m_position{ GPM::Vector3D::zero }, m_rotation{ GPM::Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = GPM::Vector3D::forward;
    m_right = GPM::Vector3D::right;
    m_up = GPM::Vector3D::up;

    Vector3D quat{ 0, 1, 0 };
    m_transform = Matrix4F::CreateTransformation(Vector3F(0.0f, 0.0f, 0.0f),
        Quaternion::CreateFromAxisAngle(quat, GPM::Tools::Utils::ToRadians(0.0f)),
        Vector3F{ 0.02f, 0.02f, 0.02f });
}

Engine::ObjectElements::Transform::Transform(GPM::Vector3D& p_position) :
    m_scale{ GPM::Vector3D::one }, m_position{ p_position }, m_rotation{ GPM::Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = GPM::Vector3D::forward;
    m_right = GPM::Vector3D::Cross(GPM::Vector3D::up, m_forward);
    m_up = GPM::Vector3D::Cross(m_forward, GPM::Vector3D::right);
}

Engine::ObjectElements::Transform::Transform(const Transform& p_other) :
    m_parent{ p_other.m_parent }, m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_up{ p_other.m_up },
    m_scale{ p_other.m_scale }, m_position{ p_other.m_position }, m_rotation{ p_other.m_rotation } {}

Engine::ObjectElements::Transform::Transform(const Transform&& p_other) noexcept :
    m_parent{ p_other.m_parent }, m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_up{ p_other.m_up },
    m_scale{ p_other.m_scale }, m_position{ p_other.m_position }, m_rotation{ p_other.m_rotation } {}

void Engine::ObjectElements::Transform::Translate(const GPM::Vector3D& p_vector)
{
    m_position += p_vector;
}


void Engine::ObjectElements::Transform::RotateWithEulerAngles(const GPM::Vector3D& p_euler)
{
    //TODO: check if it properly accepts angles > 360
    GPM::Quaternion quat;
    quat.MakeFromEuler(p_euler);
    m_rotation *= quat;
    CalculateAxes();
}

void Engine::ObjectElements::Transform::Scale(const GPM::Vector3D& p_scale)
{
    m_scale *= p_scale;
}

GPM::Vector3D Engine::ObjectElements::Transform::GetEuler() const
{
    return m_rotation.ToEuler();
}

void Engine::ObjectElements::Transform::CalculateAxes()
{
    m_forward = m_rotation.ToEuler().Normalized();
    m_right = GPM::Vector3D::Cross(GPM::Vector3D::up, m_forward);
    m_up = GPM::Vector3D::Cross(m_forward, GPM::Vector3D::right);
}
