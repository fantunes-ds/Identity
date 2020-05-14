#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Physics/Rigidbody.h>
#include <LinearMath/btTransform.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Components/Transform.h>

namespace Engine::Components
{

    class API_ENGINE BoxCollider: public IComponent
    {
    public:
        BoxCollider(Objects::GameObject* p_gameObject, std::shared_ptr<BoxCollider> p_other);
        BoxCollider(Objects::GameObject* p_gameObject);
        BoxCollider(Objects::GameObject* p_gameObject, std::vector<std::string> p_block);
        BoxCollider(const BoxCollider&) = default;
        ~BoxCollider();

        void Serialize(std::ostream& p_stream) override;
        void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) override;
        /***
         * @brief returns Bullet Physics' Matrix that concerns this BoxCollider.
         */
        [[nodiscard]] GPM::Matrix4F GetWorldMatrix() const;
        std::shared_ptr<ObjectElements::Model> GetModel() { return m_model; }
        //std::shared_ptr<ObjectElements::Transform> GetTransform() { return m_transform; }
        btRigidBody* GetBtRigidbody() { return m_rigidbody; }
        btDefaultMotionState* GetMotionState() { return m_motionState; }
        GPM::Vector3F& GetOffset() { return m_offset; }

        void SetPositionOffset(GPM::Vector3F p_offset); 
        void SetMass(float p_mass);
        void SetDimensions(const GPM::Vector3F& p_dimensions);

        bool operator==(IComponent* p_other) override { return false; }
        bool DeleteFromMemory() override;
        void SetActive(bool p_active) override;
    private:
        /**
         * @brief Builds a Model that visually represents this BoxCollider's transform. 
         */
        ObjectElements::Model ConstructBox();

        float m_mass;
        GPM::Vector3F m_offset;
        btBoxShape* m_box;
        btDefaultMotionState* m_motionState;
        btRigidBody* m_rigidbody;
        std::shared_ptr<ObjectElements::Model> m_model;
    };
}
