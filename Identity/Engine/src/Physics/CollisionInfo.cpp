#include <stdafx.h>
#include <Physics/CollisionInfo.h>
#include <Objects/GameObject.h>

Engine::Physics::CollisionInfo::CollisionInfo(const GPM::Vector3F& p_contactPoint, const GPM::Vector3F& p_contactNormal,
    Objects::GameObject* p_collision): m_contactPoint{p_contactPoint}, m_contactNormal{p_contactNormal}, m_collision{p_collision}
{

}
