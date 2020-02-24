#include <stdafx.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <Containers/TransformContainer.h>

Engine::ObjectElements::Transform::Transform() :
    m_position{ Vector3D::zero }, m_scale{ Vector3D::one }, m_rotation{ Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = Vector3D::forward;
    m_right = Vector3D::right;
    m_up = Vector3D::up;
    UpdateTransformMatrix();


}

Engine::ObjectElements::Transform::Transform(Vector3D& p_position) :
    m_position{ p_position }, m_scale{ Vector3D::one }, m_rotation{ Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = Vector3D::forward;
    m_right = Vector3D::Cross(Vector3D::up, m_forward);
    m_up = Vector3D::Cross(m_forward, Vector3D::right);
    UpdateTransformMatrix();
}

Engine::ObjectElements::Transform::Transform(const Transform& p_other) :
    m_parent{ p_other.m_parent }, m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_up{ p_other.m_up },
    m_scale{ p_other.m_scale }, m_position{ p_other.m_position }, m_rotation{ p_other.m_rotation } {}

/*Engine::ObjectElements::Transform::Transform(const Transform&& p_other) noexcept :
    m_parent{ p_other.m_parent }, m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_up{ p_other.m_up },
    m_scale{ p_other.m_scale }, m_position{ p_other.m_position }, m_rotation{ p_other.m_rotation } {}*/

void Engine::ObjectElements::Transform::Translate(const Vector3D& p_vector)
{
    m_position += p_vector;
    UpdateTransformMatrix();
}


void Engine::ObjectElements::Transform::RotateWithEulerAngles(const Vector3D& p_euler)
{
    //TODO: check if it properly accepts angles > 360
    Quaternion quat;
    quat.MakeFromEuler(p_euler);
    m_rotation *= quat;
    CalculateAxes();
    UpdateTransformMatrix();
}

void Engine::ObjectElements::Transform::Scale(const Vector3D& p_scale)
{
    m_scale *= p_scale;
    UpdateTransformMatrix();
}

void Engine::ObjectElements::Transform::UpdateTransformMatrix()
{
    m_transform = Matrix4F::CreateTransformation(m_position,
                                                          m_rotation,
                                                          m_scale);
}

Vector3D Engine::ObjectElements::Transform::GetEuler() const
{
    return m_rotation.ToEuler();
}

std::shared_ptr<Engine::ObjectElements::Transform> Engine::ObjectElements::Transform::GetParent() const
{
    return Containers::TransformContainer::FindTransform(m_parent);
}

void Engine::ObjectElements::Transform::CalculateAxes()
{
    m_forward = m_rotation.ToEuler().Normalized();
    m_right = Vector3D::Cross(Vector3D::up, m_forward);
    m_up = Vector3D::Cross(m_forward, Vector3D::right);
}
