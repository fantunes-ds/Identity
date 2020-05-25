#pragma once
#include <Export.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btAlignedObjectArray.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>

namespace Engine {
    namespace Components {
        class BoxCollider;
        class SphereCollider;
    }
}

namespace Engine::Systems
{
    class API_ENGINE PhysicsSystem
    {
    public:
        ~PhysicsSystem();

        static PhysicsSystem* GetInstance();

        static std::shared_ptr<Components::BoxCollider> AddBoxCollider(Components::BoxCollider* p_collider);
        static std::shared_ptr<Components::SphereCollider> AddSphereCollider(Components::SphereCollider* p_collider);
        static void RemoveBoxCollider(int32_t p_id);
        static void RemoveSphereCollider(int32_t p_id);

        static std::map<int32_t, std::shared_ptr<Components::BoxCollider>>& GetColliders() { return GetInstance()->m_boxColliders; }
        static std::map<int32_t, std::shared_ptr<Components::SphereCollider>>& GetSphereColliders() { return GetInstance()->m_sphereColliders; }
        static btDiscreteDynamicsWorld* GetWorld() { return GetInstance()->m_dynamicsWorld; }
        static void Update(const float p_deltaTime);
        static void FixedUpdate();
        static void BulletTickCallback(btDynamicsWorld* p_world, btScalar p_timeStep);
        static std::shared_ptr<Components::BoxCollider> FindBoxCollider(const btCollisionObject* p_collisionObject);
        static std::shared_ptr<Engine::Components::SphereCollider> FindSphereCollider(const btCollisionObject* p_collisionObject);
        static std::shared_ptr<Engine::Physics::ICollider> FindCollider(const btCollisionObject* p_collisionObject);

    private:
        PhysicsSystem();

        float m_fixedUpdateCounter = 0.0f;

        inline static PhysicsSystem* m_instance;
        std::map<int32_t, std::shared_ptr<Components::BoxCollider>> m_boxColliders;
        std::map<int32_t, std::shared_ptr<Components::SphereCollider>> m_sphereColliders;

        //TODO: create class that wraps bullet classes
        btDefaultCollisionConfiguration* m_collisionConfiguration;

        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        btCollisionDispatcher* m_dispatcher;

        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        btBroadphaseInterface* m_overlappingPairCache;

        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        btSequentialImpulseConstraintSolver* m_solver;

        btDiscreteDynamicsWorld* m_dynamicsWorld;

        //keep track of the shapes, we release memory at exit.
        //make sure to re-use collision shapes among rigid bodies whenever possible!
    };
}
