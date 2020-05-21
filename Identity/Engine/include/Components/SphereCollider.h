#pragma once
#include <Export.h>
#include <Components/IComponent.h>
#include <Physics/Rigidbody.h>
#include <LinearMath/btTransform.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btDefaultMotionState.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Components/Transform.h>

namespace Engine::Components
{

    class API_ENGINE SphereCollider: public IComponent
    {
    public:
        SphereCollider(Objects::GameObject* p_gameObject, std::shared_ptr<SphereCollider> p_other);
        SphereCollider(Objects::GameObject* p_gameObject);
        SphereCollider(Objects::GameObject* p_gameObject, std::vector<std::string> p_block);
        SphereCollider(const SphereCollider&) = default;
        ~SphereCollider();

        void Serialize(std::ostream& p_stream) override;
        void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) override;
        /***
         * @brief returns Bullet Physics' Matrix that concerns this BoxCollider.
         */
        [[nodiscard]] GPM::Matrix4F GetWorldMatrix() const;
        std::shared_ptr<ObjectElements::Model> GetModel() { return m_model; }
        btRigidBody* GetBtRigidbody() { return m_rigidbody; }
        btDefaultMotionState* GetMotionState() { return m_motionState; }
        GPM::Vector3F& GetOffset() { return m_offset; }
        float& GetMass() { return m_mass; }

        void SetPositionOffset(GPM::Vector3F p_offset); 
        void SetMass(float p_mass);
        void SetRadius(float p_radius);

        //Not yet functional
        //void SetScale(const GPM::Vector3F& p_scale);
        
        bool operator==(IComponent* p_other) override { return false; }
        bool DeleteFromMemory() override;
        void SetActive(bool p_active) override;
    private:
        /**
         * @brief Builds a Model that visually represents this SphereCollider's transform. 
         */
        ObjectElements::Model ConstructSphere();

        float m_mass;
        Vector3F m_offset;
        Vector3F m_scale = Vector3F::one;
        btSphereShape* m_sphere;
        btDefaultMotionState* m_motionState;
        btRigidBody* m_rigidbody;
        std::shared_ptr<ObjectElements::Model> m_model;
    };
}
