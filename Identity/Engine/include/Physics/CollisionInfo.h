#pragma once
#include <Export.h>

namespace Engine::Objects
{
    class GameObject;

}
namespace Engine::Physics
{
    class API_ENGINE CollisionInfo
    {
    public:
        CollisionInfo(const GPM::Vector3F& p_contactPoint, const GPM::Vector3F& p_contactNormal, Objects::GameObject* p_collision);
        ~CollisionInfo() = default;

        const GPM::Vector3F& GetContactPoint() const { return m_contactPoint; }
        const GPM::Vector3F& GetContactNormal() const { return m_contactNormal; }
        Objects::GameObject* GetCollision() const { return m_collision; }

    private:
        GPM::Vector3F m_contactPoint;
        GPM::Vector3F m_contactNormal;
        Objects::GameObject* m_collision;
    };
}
