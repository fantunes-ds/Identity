#pragma once
#include <Export.h>
#include <Physics/CollisionInfo.h>

namespace Engine::Physics
{
    class API_ENGINE ICollider: public Components::IComponent
    {
    public:
        ICollider(Objects::GameObject* p_gameObject, Engine::Components::Type p_type): IComponent{p_gameObject, p_type} {}
        virtual ~ICollider() = default;

        virtual void OnCollisionEnter();
        virtual void OnCollisionStay();
        virtual void OnCollisionExit();

        void ActOnCollisionInfo();

        bool GetHasCollidedThisFrame() const { return m_hasCollidedThisFrame; }
        bool GetHasCollidedLastFrame() const { return m_hasCollidedLastFrame; }
        std::shared_ptr<CollisionInfo> GetCollisionInfo() const { return m_collisionInfo; }
        bool IsColliding();

        void SetHasCollidedThisFrame(bool p_coll) { m_hasCollidedThisFrame = p_coll; }
        void SetHasCollidedLastFrame(bool p_coll) { m_hasCollidedLastFrame = p_coll; }
        void SetCollisionInfo(std::shared_ptr<Physics::CollisionInfo> p_collisionInfo) { m_collisionInfo = p_collisionInfo; }

    protected:
        bool m_hasCollidedLastFrame = false;
        bool m_hasCollidedThisFrame = false;
        std::shared_ptr<CollisionInfo> m_collisionInfo = nullptr;
    };
}
