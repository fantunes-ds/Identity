#include <stdafx.h>
#include <3DLoader/ObjectElements/Transform.h>

Engine::ObjectElements::Transform::Transform():
    m_position{ GPM::Vector3D::zero }, m_scale{ GPM::Vector3D::one }, m_rotation{ GPM::Quaternion{} }
{
    m_forward = {0.0, 0.0, 1.0};
    m_right = m_forward.right;
}

Engine::ObjectElements::Transform::Transform(GPM::Vector3D& p_position, GPM::Vector3D& p_scale, GPM::Vector3D& p_rotation) :
    m_position{ p_position }, m_scale{ p_scale }, m_forward{ p_rotation.Normalized() }
{
    //TODO: check if it properly accepts angles > 360
    GPM::Quaternion quat;
    quat.MakeFromEuler(p_rotation.x, p_rotation.y, p_rotation.z);
    m_rotation = quat;
    m_right = GPM::Vector3D::Cross(m_forward.up, m_forward);
}

Engine::ObjectElements::Transform::Transform(const Transform& p_other):
    m_position{ p_other.m_position }, m_scale{ p_other.m_scale }, m_rotation{ p_other.m_rotation },
    m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_parent{ p_other.m_parent } {}

Engine::ObjectElements::Transform::Transform(const Transform&& p_other) noexcept:
    m_position{ p_other.m_position }, m_scale{ p_other.m_scale }, m_rotation{ p_other.m_rotation },
    m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_parent{ p_other.m_parent } {}

void Engine::ObjectElements::Transform::Translate(const GPM::Vector3D& p_vector)
{
    m_position += p_vector;
}


void Engine::ObjectElements::Transform::RotateWithEulerAngles(const GPM::Vector3D& p_euler)
{
    GPM::Quaternion quat;
    quat.MakeFromEuler(p_euler);
    m_rotation *= quat;
}

void Engine::ObjectElements::Transform::Scale(const GPM::Vector3D& p_scale)
{
    m_scale *= p_scale;
}

GPM::Vector3D Engine::ObjectElements::Transform::GetEuler() const
{
    return m_rotation.ToEuler();
}
