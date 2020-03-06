#pragma once
#include <Export.h>
#include <Physics/BulletInclude.h>
#include <Rendering/Materials/Shader.h>

namespace Engine::Tools::Bullet
{
    class API_ENGINE BulletDebugRenderer: public btIDebugDraw
    {
    public:
        BulletDebugRenderer();
        ~BulletDebugRenderer() = default;

        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {}
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {}
        void reportErrorWarning(const char* warningString) override {}
        void draw3dText(const btVector3& location, const char* textString) override {}
        void setDebugMode(int debugMode) override {}
        int getDebugMode() const override { return 0; }

        GPM::Matrix4F GetWorldMatrix();
        static BulletDebugRenderer* GetInstance();

    private:
        static inline BulletDebugRenderer* m_instance;

        Rendering::Materials::Shader m_shader;
        btCollisionShape* m_box;
        btTransform m_transform;
        btRigidBody* m_body;
        btDefaultMotionState* m_motionState;
    };
}
