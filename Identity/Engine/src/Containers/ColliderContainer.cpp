#include <stdafx.h>
#include <Containers/ColliderContainer.h>
#include <Components/BoxCollider.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <Objects/GameObject.h>
#include "Containers/TransformContainer.h"

Engine::Containers::ColliderContainer::~ColliderContainer()
{
    delete m_instance;
}

Engine::Containers::ColliderContainer* Engine::Containers::ColliderContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ColliderContainer();
    }

    return m_instance;
}

std::shared_ptr<Engine::Components::BoxCollider> Engine::Containers::ColliderContainer::AddCollider(
    Components::BoxCollider& p_collider)
{
    auto coll = std::make_shared<Components::BoxCollider>(p_collider);
    GetInstance()->m_colliders.insert_or_assign(p_collider.GetID(), coll);
    GetInstance()->m_dynamicsWorld->addRigidBody(coll->GetBtRigidbody());
    return coll;
}

void Engine::Containers::ColliderContainer::Update(float p_deltaTime)
{
    btTransform trans;

    GetInstance()->m_dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    for (auto& collider : GetInstance()->m_colliders)
    {
        collider.second->GetMotionState()->getWorldTransform(trans);
        
        btVector3& collPos = trans.getOrigin();
        btQuaternion collRot = trans.getRotation();

        collider.second->GetGameObject()->GetTransform()->SetPosition(GPM::Vector3F(collPos.getX(), collPos.getY(), collPos.getZ()));
        collider.second->GetGameObject()->GetTransform()->SetRotation(GPM::Quaternion(collRot.getX(), collRot.getY(), collRot.getZ(), -collRot.getW()));

        collider.second->GetGameObject()->GetTransform()->UpdateWorldTransformMatrix();
    }
}

Engine::Containers::ColliderContainer::ColliderContainer()
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
