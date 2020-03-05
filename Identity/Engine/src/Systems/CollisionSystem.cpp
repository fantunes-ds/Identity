#include <stdafx.h>
#include <Systems/CollisionSystem.h>

Engine::Systems::CollisionSystem::CollisionSystem()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

   m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    ///-----initialization_end-----
}

Engine::Systems::CollisionSystem::~CollisionSystem()
{
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_dynamicsWorld;
    delete m_overlappingPairCache;
    delete m_solver;

    for (int i = 0; i < m_collisionShapes.size(); ++i)
        delete m_collisionShapes[i];
}
