#pragma once
#include <Export.h>
#include <Physics/BulletInclude.h>
#include <Tools/Bullet/BulletDebugRender.h>

namespace Engine::Systems
{
    class API_ENGINE CollisionSystem
    {
    public:
        CollisionSystem();
        ~CollisionSystem();

        void Update(float p_deltaTime);
        void InitTestScene();

    private:

        Tools::Bullet::BulletDebugRenderer* m_debugRenderer;

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
