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

bool Engine::Physics::ICollider::IsColliding()
{
    if (m_hasCollidedThisFrame && m_hasCollidedLastFrame)
        return true;

    return false;
}
