#include <stdafx.h>
#include <Tools/Bullet/BulletDebugRender.h>
#include <winsetup.h>
#include "Tools/DirectX/GraphicsMacros.h"
#include <d3dcompiler.h>
#include <Systems/CameraSystem.h>
#include "Containers/ModelContainer.h"

Engine::Tools::Bullet::BulletDebugRenderer::BulletDebugRenderer()
{
    m_box = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    m_box->setLocalScaling(btVector3());

    m_transform.setIdentity();
    m_transform.setOrigin(btVector3(0, 0, 0));

    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    m_motionState = new btDefaultMotionState(m_transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0, m_motionState, m_box, localInertia);
    m_body = new btRigidBody(rbInfo);

    btVector3 a;
    btVector3 b;

    m_box->getAabb(m_transform, a, b);
    std::vector<Geometry::Vertex> vertices;
    std::vector<unsigned short> indices;

    vertices.emplace_back(Geometry::Vertex{ GPM::Vector3F{a.getX(), a.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{} });
    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{b.getX(), a.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});
    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{b.getX(), b.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});
    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{a.getX(), b.getY(), a.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});

    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{a.getX(), a.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});
    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{b.getX(), a.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});
    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{b.getX(), b.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});
    vertices.emplace_back(Geometry::Vertex{GPM::Vector3F{a.getX(), b.getY(), b.getZ()}, GPM::Vector2F{}, GPM::Vector3F{}});

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
    Containers::ModelContainer::AddModel(model);
}

GPM::Matrix4F Engine::Tools::Bullet::BulletDebugRenderer::GetWorldMatrix()
{
    btScalar m[16];

    m_transform.getOpenGLMatrix(m);
    Matrix4F mat
    {
        m[0], m[1], m[2], m[3],
        m[4], m[5], m[6], m[7],
        m[8], m[9], m[10], m[11],
        m[12], m[13], m[14], m[15]
    };

    return mat.Transpose();
}
