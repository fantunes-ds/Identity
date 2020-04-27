#include <stdafx.h>
#include <Components/BoxCollider.h>
#include <LinearMath/btVector3.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <Objects/GameObject.h>
#include <Containers/ColliderContainer.h>

Engine::Components::BoxCollider::BoxCollider(Objects::GameObject* p_gameObject) : IComponent{ p_gameObject }
{
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    m_box = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    btTransform trans;
    auto& position = m_gameObject->GetTransform()->GetPosition();
    auto& rotation = m_gameObject->GetTransform()->GetRotation();
    auto& scale = m_gameObject->GetTransform()->GetScale();

    trans.setIdentity();

    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(btQuaternion(rotation.GetXAxisValue(), rotation.GetYAxisValue(), rotation.GetZAxisValue(), rotation.w));

    m_box->calculateLocalInertia(m_mass, localInertia);

    m_motionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_box, localInertia);
    m_rigidbody = new btRigidBody(rbInfo);

    ObjectElements::Model model = ConstructBox();

    //TODO add a "AddModel" that take a model as parametre
    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);

    Containers::ColliderContainer::AddCollider(this);

}

Engine::Components::BoxCollider::~BoxCollider()
{
    delete m_box;
    delete m_rigidbody;
    delete m_motionState;
}

GPM::Matrix4F Engine::Components::BoxCollider::GetWorldMatrix() const
{
    btScalar m[16];
    btTransform trans;

    m_motionState->getWorldTransform(trans);
    trans.getOpenGLMatrix(m);

    Matrix4F mat
    {
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]
    };

    return mat.Transpose();
}

void Engine::Components::BoxCollider::SetPositionOffset(GPM::Vector3F& p_offset)
{
    m_offset = p_offset;
    btTransform trans;
}

void Engine::Components::BoxCollider::SetMass(float p_mass)
{
    m_mass = p_mass;

    if (m_rigidbody)
    {
        Containers::ColliderContainer::GetWorld()->removeRigidBody(m_rigidbody);
    }

    auto localInertia = btVector3(0.0f, 0.0f, 0.0f);

    if (m_box)
        m_box->calculateLocalInertia(m_mass, localInertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_box, localInertia);
    delete m_rigidbody;
    m_rigidbody = new btRigidBody(rbInfo);
    Containers::ColliderContainer::GetWorld()->addRigidBody(m_rigidbody);
}

void Engine::Components::BoxCollider::SetDimensions(const GPM::Vector3F& p_dimensions)
{
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (m_rigidbody)
    {
        delete m_box;

        Containers::ColliderContainer::GetWorld()->removeRigidBody(m_rigidbody);
        m_box = new btBoxShape(btVector3(p_dimensions.x, p_dimensions.y, p_dimensions.z));
        m_rigidbody->setCollisionShape(m_box);
        m_box->calculateLocalInertia(m_mass, localInertia);
        m_rigidbody->setMassProps(m_mass, localInertia);
        m_rigidbody->updateInertiaTensor();
        Containers::ColliderContainer::GetWorld()->addRigidBody(m_rigidbody);
    }

    ObjectElements::Model model = ConstructBox();
    Managers::ResourceManager::RemoveModel(m_model->GetID());
    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);
}

Engine::ObjectElements::Model Engine::Components::BoxCollider::ConstructBox()
{
    btVector3 a;
    btVector3 b;
    btTransform trans;

    m_motionState->getWorldTransform(trans);
    m_box->getAabb(trans, a, b);

    std::vector<Geometry::Vertex> vertices;
    std::vector<unsigned short> indices;

    for (int i = 0; i < m_box->getNumVertices(); ++i)
    {
        btVector3 vertex;
        m_box->getVertex(i, vertex);
        vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{vertex.getX(), vertex.getY(), vertex.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    }

    //Back

    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);

    indices.emplace_back(2);
    indices.emplace_back(3);
    indices.emplace_back(0);


    //Left
    indices.emplace_back(4);
    indices.emplace_back(0);
    indices.emplace_back(2);

    indices.emplace_back(2);
    indices.emplace_back(6);
    indices.emplace_back(4);

    //bottom
    indices.emplace_back(0);
    indices.emplace_back(4);
    indices.emplace_back(5);

    indices.emplace_back(5);
    indices.emplace_back(1);
    indices.emplace_back(0);

    //right
    indices.emplace_back(5);
    indices.emplace_back(1);
    indices.emplace_back(3);

    indices.emplace_back(3);
    indices.emplace_back(7);
    indices.emplace_back(5);

    //top
    indices.emplace_back(7);
    indices.emplace_back(3);
    indices.emplace_back(2);

    indices.emplace_back(2);
    indices.emplace_back(6);
    indices.emplace_back(7);

    //front
    indices.emplace_back(6);
    indices.emplace_back(5);
    indices.emplace_back(4);

    indices.emplace_back(4);
    indices.emplace_back(7);
    indices.emplace_back(6);

    ObjectElements::Model model;

    auto mesh = std::make_shared<ObjectElements::Mesh>(vertices, indices);

    model.AddMesh(mesh);
    return model;
}

