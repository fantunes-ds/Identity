#include <stdafx.h>
#include <Components/SphereCollider.h>
#include <Objects/GameObject.h>

#include "Components/Sound.h"

void Engine::Physics::ICollider::OnCollisionEnter()
{
    std::string str(m_gameObject->GetName() + " On collision enter with " + m_collisionInfo->GetCollision()->GetName() + "\n");
    OutputDebugString(str.c_str());
}

void Engine::Physics::ICollider::OnCollisionStay()
{
    std::string str(m_gameObject->GetName() + " On collision stay with " + m_collisionInfo->GetCollision()->GetName() + "\n");
    OutputDebugString(str.c_str());
}

void Engine::Physics::ICollider::OnCollisionExit()
{
    std::string str(m_gameObject->GetName() + " On collision exit " +  "\n");
    OutputDebugString(str.c_str());
}

void Engine::Physics::ICollider::ActOnCollisionInfo()
{
    if (m_hasCollidedThisFrame && m_hasCollidedLastFrame)
        OnCollisionStay();
    else if (m_hasCollidedThisFrame && !m_hasCollidedLastFrame)
        OnCollisionEnter();
    else if (!m_hasCollidedThisFrame && m_hasCollidedLastFrame)
    {
        m_collisionInfo = nullptr;
        OnCollisionExit();
    }
}


Matrix4F Engine::Physics::ICollider::GetWorldMatrix() const
{
    btScalar    m[16];
    btTransform trans;

    m_motionState->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);

    Matrix4F mat
    {
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]
    };

    return mat.Transpose();
}

bool Engine::Physics::ICollider::IsColliding()
{
    if (m_hasCollidedThisFrame && m_hasCollidedLastFrame)
        return true;

    return false;
}
