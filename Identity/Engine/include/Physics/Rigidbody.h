#pragma once
#include <Export.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace Engine::Physics
{
    /**
     * @warning Not ready yet, not in use anywhere
     */
    class API_ENGINE Rigidbody
    {
    public:
        Rigidbody();
        ~Rigidbody();

    private:
        btRigidBody* m_body;
    };
}
