#include <stdafx.h>
#include <Systems/CollisionSystem.h>
#include <windows.h>

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

    m_debugRenderer = Tools::Bullet::BulletDebugRenderer::GetInstance();

    m_dynamicsWorld->setDebugDrawer(m_debugRenderer);

    ///-----initialization_end-----
}

Engine::Systems::CollisionSystem::~CollisionSystem()
{
    delete m_debugRenderer;

    for (int i = 0; i < m_collisionShapes.size(); ++i)
        delete m_collisionShapes[i];

    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_overlappingPairCache;
    delete m_solver;
    //delete m_dynamicsWorld;
}

void Engine::Systems::CollisionSystem::Update(float p_deltaTime)
{

    m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    //print positions of all objects
    for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }

        std::string str;

        if (j == 0)
            str = ("world pos floor " + std::to_string(j) + " = " + std::to_string(trans.getOrigin().getX()) + ", " + std::to_string(trans.getOrigin().getY()) + ", " + std::to_string(trans.getOrigin().getZ()) + "\n");
        else
            str = ("world pos sphere " + std::to_string(j) + " = " + std::to_string(trans.getOrigin().getX()) + ", " + std::to_string(trans.getOrigin().getY()) + ", " + std::to_string(trans.getOrigin().getZ()) + "\n");

        OutputDebugString(str.c_str());
    }

    m_dynamicsWorld->debugDrawWorld();
}

void Engine::Systems::CollisionSystem::InitTestScene()
{
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

    //GROUND
    btCollisionShape* groundShape = new btBoxShape(btVector3(50.0f, 50.0f, 50.0f));
    m_collisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, -56, 0));

    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0, myMotionState, groundShape, localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);

    //add the body to the dynamics world
    m_dynamicsWorld->addRigidBody(body);

    //SPHERE
    btCollisionShape* colShape = new btSphereShape(btScalar(1.));
    m_collisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar mass(1.f);
    startTransform.setOrigin(btVector3(2, 10, 0));

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* SphereMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo sphereRbInfo(mass, SphereMotionState, colShape, localInertia);
    btRigidBody* sphere = new btRigidBody(sphereRbInfo);

    m_dynamicsWorld->addRigidBody(sphere);

}
