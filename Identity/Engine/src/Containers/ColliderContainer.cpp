#include <stdafx.h>
#include <Containers/ColliderContainer.h>
#include <Components/BoxCollider.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <Objects/GameObject.h>
#include <Tools/Time.h>

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
    Components::BoxCollider* p_collider)
{
    auto coll = std::shared_ptr<Components::BoxCollider>(p_collider);
    GetInstance()->m_colliders.insert_or_assign(p_collider->GetID(), coll);
    GetInstance()->m_dynamicsWorld->addRigidBody(coll->GetBtRigidbody());
    return coll;
}

void Engine::Containers::ColliderContainer::Update(const float p_deltaTime)
{
    for (auto& collider : GetInstance()->m_colliders)
    {
        
    }
}

void Engine::Containers::ColliderContainer::FixedUpdate()
{
    btTransform trans;

    GetInstance()->m_dynamicsWorld->stepSimulation(1.0f / 60.0f, 0);

    for (auto& collider : GetInstance()->m_colliders)
    {
        //TODO: wrap all Bullet math variables
        collider.second->GetMotionState()->getWorldTransform(trans);
        GPM::Vector3F offset = collider.second->GetOffset();

        btVector3& collPos = trans.getOrigin();
        btQuaternion collRot = trans.getRotation();
        btQuaternion quatOffset(offset.x, offset.y, offset.z, 0);
        btQuaternion qpq = collRot * quatOffset * collRot.inverse();

        collider.second->GetGameObject()->GetTransform()->SetPosition(GPM::Vector3F(qpq.getX(), qpq.getY(), qpq.getZ()) + GPM::Vector3F(collPos.getX(), collPos.getY(), collPos.getZ()));
        collider.second->GetGameObject()->GetTransform()->SetRotation(GPM::Quaternion(collRot.getX(), collRot.getY(), collRot.getZ(), collRot.getW()));

        collider.second->GetGameObject()->GetTransform()->UpdateWorldTransformMatrix();
    }
}
