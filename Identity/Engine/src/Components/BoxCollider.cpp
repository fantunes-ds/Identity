#include <stdafx.h>
#include <Components/BoxCollider.h>
#include <LinearMath/btVector3.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <Objects/GameObject.h>
#include <Containers/ColliderContainer.h>

Engine::Components::BoxCollider::BoxCollider(Objects::GameObject* p_gameObject): IComponent{ p_gameObject }
{
    m_box = new btBoxShape(btVector3(10.0f, 1.0f, 10.0f));

    btTransform trans;
    trans.setIdentity();
    auto& position = m_gameObject->GetTransform()->GetPosition();
    auto& scale = m_gameObject->GetTransform()->GetScale();

    trans.setOrigin(btVector3(position.x * scale.x, position.y * scale.y, position.z * scale.z));
    //trans.setOrigin(btVector3(position.x, position.y, position.z));

    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    m_motionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(1, m_motionState, m_box, localInertia);
    m_rigidbody = new btRigidBody(rbInfo);

    ObjectElements::Model model = ConstructBox();

    m_model = Containers::ModelContainer::AddModel(model);
    m_transform = m_gameObject->GetTransform();
    Containers::ColliderContainer::AddCollider(*this);
    
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

Engine::ObjectElements::Model Engine::Components::BoxCollider::ConstructBox()
{
    btVector3 a;
    btVector3 b;
    btTransform trans;
    auto& scale = m_gameObject->GetTransform()->GetScale();

    m_motionState->getWorldTransform(trans);
    m_box->getAabb(trans, a, b);

    std::vector<Geometry::Vertex> vertices;
    std::vector<unsigned short> indices;

    /*vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX(), a.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX(), a.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX(), b.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX(), b.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });

    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX(), a.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX(), a.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX(), b.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX(), b.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });*/

    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX() * scale.x, a.getY() * scale.y, a.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX() * scale.x, a.getY() * scale.y, a.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX() * scale.x, b.getY() * scale.y, a.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX() * scale.x, b.getY() * scale.y, a.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });

    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX() * scale.x, a.getY() * scale.y, b.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX() * scale.x, a.getY() * scale.y, b.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{b.getX() * scale.x, b.getY() * scale.y, b.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX() * scale.x, b.getY() * scale.y, b.getZ() * scale.z}, GPM::Vector2F{}, GPM::Vector3F{} });

    //Back
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);


    indices.emplace_back(2);
    indices.emplace_back(3);
    indices.emplace_back(0);

    //Left
    indices.emplace_back(0);
    indices.emplace_back(3);
    indices.emplace_back(7);

    indices.emplace_back(7);
    indices.emplace_back(4);
    indices.emplace_back(0);

    //bottom
    indices.emplace_back(0);
    indices.emplace_back(4);
    indices.emplace_back(5);

    indices.emplace_back(5);
    indices.emplace_back(1);
    indices.emplace_back(0);

    //right
    indices.emplace_back(2);
    indices.emplace_back(1);
    indices.emplace_back(5);

    indices.emplace_back(5);
    indices.emplace_back(6);
    indices.emplace_back(2);

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

