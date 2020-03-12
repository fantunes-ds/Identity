#pragma once
#include <Export.h>
#include <Physics/BulletInclude.h>
#include <Rendering/Materials/Shader.h>

namespace Engine::Tools::Bullet
{
    class API_ENGINE BulletDebugRenderer
    {
    public:
        BulletDebugRenderer();
        ~BulletDebugRenderer() = default;


        GPM::Matrix4F GetWorldMatrix();
        //static BulletDebugRenderer* GetInstance();

    private:
        static inline BulletDebugRenderer* m_instance;

        btCollisionShape* m_box;
        btTransform m_transform;
        btRigidBody* m_body;
        btDefaultMotionState* m_motionState;
    };
}
