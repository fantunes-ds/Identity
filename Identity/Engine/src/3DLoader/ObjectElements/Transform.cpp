#include <stdafx.h>
#include <3DLoader/ObjectElements/Transform.h>
#include <Containers/TransformContainer.h>

Engine::ObjectElements::Transform::Transform() :
    m_position{ Vector3F::zero }, m_scale{ Vector3F::one }, m_rotation{ Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = Vector3F::forward;
    m_right = Vector3F::right;
    m_up = Vector3F::up;
    UpdateWorldTransformMatrix();


}

Engine::ObjectElements::Transform::Transform(Vector3F& p_position) :
    m_position{ p_position }, m_scale{ Vector3F::one }, m_rotation{ Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    m_forward = Vector3F::forward;
    m_right = Vector3F::Cross(Vector3F::up, m_forward);
    m_up = Vector3F::Cross(m_forward, Vector3F::right);
    UpdateWorldTransformMatrix();
}

Engine::ObjectElements::Transform::Transform(const Transform& p_other) :
    m_parent{ p_other.m_parent }, m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_up{ p_other.m_up },
    m_scale{ p_other.m_scale }, m_position{ p_other.m_position }, m_rotation{ p_other.m_rotation } {}

/*Engine::ObjectElements::Transform::Transform(const Transform&& p_other) noexcept :
    m_parent{ p_other.m_parent }, m_forward{ p_other.m_forward }, m_right{ p_other.m_right }, m_up{ p_other.m_up },
    m_scale{ p_other.m_scale }, m_position{ p_other.m_position }, m_rotation{ p_other.m_rotation } {}*/

void Engine::ObjectElements::Transform::Translate(const Vector3F& p_vector)
{

    m_position += Vector3F{ p_vector.x * - 1.0f, p_vector.y, p_vector.z * - 1.0f };
    UpdateWorldTransformMatrix();
}


void Engine::ObjectElements::Transform::RotateWithEulerAngles(const Vector3F& p_euler)
{
    //TODO: check if it properly accepts angles > 360
    Quaternion quat;
    quat.MakeFromEuler(p_euler);
    m_rotation *= quat;
    CalculateAxes();
    UpdateWorldTransformMatrix();
}

void Engine::ObjectElements::Transform::Scale(const Vector3F& p_scale)
{
    m_scale *= p_scale;
    UpdateWorldTransformMatrix();
}

void Engine::ObjectElements::Transform::UpdateWorldTransformMatrix()
{
    m_worldTransform = Matrix4F::CreateTransformation(m_position,
                                                          m_rotation,
                                                          m_scale);
}

Vector3F Engine::ObjectElements::Transform::GetEuler() const
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
    m_right = Vector3F::Cross(Vector3F::up, m_forward);
    m_up = Vector3F::Cross(m_forward, Vector3F::right);
}
