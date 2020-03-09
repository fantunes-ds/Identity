#pragma once
#include <Export.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace Engine::Physics
{
    class API_ENGINE Rigidbody
    {
    public:
        Rigidbody();
        ~Rigidbody();

    private:
        btRigidBody* m_body;
    };
}
