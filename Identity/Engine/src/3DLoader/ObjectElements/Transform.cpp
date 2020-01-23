#include <stdafx.h>
#include <3DLoader/ObjectElements/Transform.h>

Engine::ObjectElements::Transform::Transform() :
    m_position{ GPM::Vector3D::zero }, m_scale{ GPM::Vector3D::one }, m_rotation{ GPM::Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = GPM::Vector3D::forward;
    m_right = GPM::Vector3D::right;
    m_up = GPM::Vector3D::up;
}

Engine::ObjectElements::Transform::Transform(GPM::Vector3D& p_position) :
    m_position{ p_position }, m_scale{ GPM::Vector3D::one }, m_rotation{ GPM::Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = GPM::Vector3D::forward;
    m_right = GPM::Vector3D::Cross(GPM::Vector3D::up, m_forward);
    m_up = GPM::Vector3D::Cross(m_forward, GPM::Vector3D::right);

    //m_transform = Matrix4F::CreateTransformation(p_position, m_rotation, m_scale);
    /*Vector4F r(Vector3F::right);
    Vector4F u(Vector3F::up);
    Vector4F f(Vector3F::forward);

    r.w = 0.0f;
    u.w = 0.0f;
    f.w = 0.0f;

    m_axes.SetRow(0, r);
    m_axes.SetRow(1, u);
    m_axes.SetRow(2, f);
    m_axes.SetRow(3, {0, 0, 0, 1});*/
    
}

Engine::ObjectElements::Transform::Transform(const Transform& p_other) :
    m_position{ p_other.m_position }, m_scale{ p_other.m_scale }, m_rotation{ p_other.m_rotation },
    m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_parent{ p_other.m_parent } {}

Engine::ObjectElements::Transform::Transform(const Transform&& p_other) noexcept :
    m_position{ p_other.m_position }, m_scale{ p_other.m_scale }, m_rotation{ p_other.m_rotation },
    m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_parent{ p_other.m_parent } {}

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
    /*m_axes.Rotate(m_rotation);

    m_right = Vector3D(m_axes[0], m_axes[1], m_axes[2]);
    m_up = Vector3D(m_axes[4], m_axes[5], m_axes[6]);
    m_forward = Vector3D(m_axes[8], m_axes[9], m_axes[10]);*/

    m_forward = m_rotation.ToEuler().Normalized();
    m_right = GPM::Vector3D::Cross(GPM::Vector3D::up, m_forward);
    m_up = GPM::Vector3D::Cross(m_forward, GPM::Vector3D::right);
}
