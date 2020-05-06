#include <stdafx.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>

#include <Components/BoxCollider.h>
#include <Objects/GameObject.h>
#include <Systems/PhysicsSystem.h>
#include <Tools/Time.h>

Engine::Systems::PhysicsSystem::PhysicsSystem()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    m_overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    m_solver = new btSequentialImpulseConstraintSolver;

    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

    m_dynamicsWorld->setGravity(btVector3(0.0f, -10.0f, 0.0f));
}

Engine::Systems::PhysicsSystem::~PhysicsSystem()
{
    delete m_instance;
}

Engine::Systems::PhysicsSystem* Engine::Systems::PhysicsSystem::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new PhysicsSystem();
    }

    return m_instance;
}

std::shared_ptr<Engine::Components::BoxCollider> Engine::Systems::PhysicsSystem::AddCollider(Components::BoxCollider* p_collider)
{
    auto coll = std::shared_ptr<Components::BoxCollider>(p_collider);
    GetInstance()->m_colliders.insert_or_assign(p_collider->GetID(), coll);
    GetInstance()->m_dynamicsWorld->addRigidBody(coll->GetBtRigidbody());
    return coll;
}

void Engine::Systems::PhysicsSystem::RemoveCollider(int32_t p_id)
{
    GetInstance()->m_colliders.erase(p_id);
}

void Engine::Systems::PhysicsSystem::Update(const float p_deltaTime)
{
    for (auto& collider : GetInstance()->m_colliders)
    {
    }
}

void Engine::Systems::PhysicsSystem::FixedUpdate()
{
    btTransform trans;

    GetInstance()->m_dynamicsWorld->stepSimulation(1.0f / 60.0f, 0);

    auto colliders = GetInstance()->m_colliders;
    std::cout << "this is for test";

    for (auto& collider : GetInstance()->m_colliders)
    {
        if (collider.second->IsActive())
        {
            //TODO: wrap all Bullet math variables
            collider.second->GetMotionState()->getWorldTransform(trans);
            Vector3F offset = collider.second->GetOffset();

            btVector3&   collPos = trans.getOrigin();
            btQuaternion collRot = trans.getRotation();
            btQuaternion quatOffset(offset.x, offset.y, offset.z, 0);
            btQuaternion qpq = collRot * quatOffset * collRot.inverse();

            collider.second->GetGameObject()->GetTransform()->SetPosition(Vector3F(qpq.getX(), qpq.getY(), qpq.getZ()) +
                                                                          Vector3F(collPos.getX(), collPos.getY(), collPos.getZ()));
            collider.second->GetGameObject()->GetTransform()->SetRotation(Quaternion(collRot.getX(), collRot.getY(),
                                                                                          collRot.getZ(), collRot.getW()));

            collider.second->GetGameObject()->GetTransform()->UpdateWorldTransformMatrix();
        }
    }
}
