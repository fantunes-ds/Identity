#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Physics/Rigidbody.h>
#include <LinearMath/btTransform.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <3DLoader/ObjectElements/Model.h>
#include "3DLoader/ObjectElements/Transform.h"

namespace Engine::Components
{

    class API_ENGINE BoxCollider: public IComponent
    {
    public:
        BoxCollider(Objects::GameObject* p_gameObject);
        BoxCollider(const BoxCollider&) = default;
        ~BoxCollider();

        [[nodiscard]] GPM::Matrix4F GetWorldMatrix() const;
        std::shared_ptr<ObjectElements::Model> GetModel() { return m_model; }
        std::shared_ptr<ObjectElements::Transform> GetTransform() { return m_transform; }
        btRigidBody* GetBtRigidbody() { return m_rigidbody; }
        btDefaultMotionState* GetMotionState() { return m_motionState; }
        //btTransform& GetBtTransform() { return m_btTransform; }

        bool operator==(IComponent* p_other) override { return false; }
        bool DeleteFromMemory() override { return false; }


    private:
        ObjectElements::Model ConstructBox();

        btBoxShape* m_box;
        btDefaultMotionState* m_motionState;
        btRigidBody* m_rigidbody;
        std::shared_ptr<ObjectElements::Model> m_model;
        std::shared_ptr<ObjectElements::Transform> m_transform;
    };
}
