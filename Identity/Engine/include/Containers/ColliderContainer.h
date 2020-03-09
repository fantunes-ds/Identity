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
    }
}

namespace Engine::Containers
{
    class API_ENGINE ColliderContainer
    {
    public:
        ~ColliderContainer();

        static ColliderContainer* GetInstance();

        static std::shared_ptr<Components::BoxCollider> AddCollider(Components::BoxCollider& p_collider);
        static std::map<int32_t, std::shared_ptr<Components::BoxCollider>>& GetColliders() { return GetInstance()->m_colliders; }
        static void Update(float p_deltaTime);

    private:
        ColliderContainer();

        inline static ColliderContainer* m_instance;
        std::map<int32_t, std::shared_ptr<Components::BoxCollider>> m_colliders;

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
        btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
    };
}
