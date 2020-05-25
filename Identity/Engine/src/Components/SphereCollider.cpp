#include <stdafx.h>

#include <LinearMath/btVector3.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <Components/SphereCollider.h>
#include <Objects/GameObject.h>
#include <Systems/PhysicsSystem.h>
#include <Managers/ResourceManager.h>

Engine::Components::SphereCollider::SphereCollider(Objects::GameObject* p_gameObject) : ICollider{ p_gameObject, SPHERE_COLLIDER }
{
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    m_sphere = new btSphereShape(1.0f);
    btTransform trans;
    auto       position = m_gameObject->GetTransform()->GetPosition();
    auto       rotation = m_gameObject->GetTransform()->GetRotation();

    trans.setIdentity();

    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(btQuaternion(rotation.GetXAxisValue(), rotation.GetYAxisValue(), rotation.GetZAxisValue(), rotation.w));

    m_sphere->calculateLocalInertia(m_mass, localInertia);

    m_motionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_sphere, localInertia);
    m_rigidbody = new btRigidBody(rbInfo);

    ObjectElements::Model model = ConstructSphere();

    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);

    Systems::PhysicsSystem::AddCollider(this);
}

Engine::Components::SphereCollider::SphereCollider(Engine::Objects::GameObject* p_gameObject, std::vector<std::string> p_block) : ICollider{ p_gameObject, Engine::Components::SPHERE_COLLIDER }
{
    std::vector<std::string> words;

    for (int i = 0; i < p_block.size(); ++i)
    {
        std::stringstream stringStream(p_block[i]);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (words[0] == "m_mass")
        {
            m_mass = std::stof(words[1]);
        }
        else if (words[0] == "m_offset")
        {
            m_offset.x = std::stof(words[1]);
            m_offset.y = std::stof(words[2]);
            m_offset.z = std::stof(words[3]);
        }
        else if (words[0] == "m_radius")
        {
            m_sphere = new btSphereShape(std::stof(words[1]));
        }

        words.clear();
    }

    //TODO: Memory leak here
    /*delete m_sphere;
    delete m_rigidbody;
    delete m_motionState;*/

    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    btTransform trans;
    auto position = m_gameObject->GetTransform()->GetPosition();
    auto rotation = m_gameObject->GetTransform()->GetRotation();

    Vector3D offsetD = m_offset;
    Quaternion q = rotation * offsetD * Quaternion::Conjugate(rotation);
    offsetD = { q.GetXAxisValue(), q.GetYAxisValue(), q.GetZAxisValue() };
    Vector3D offset = offsetD;
    position += offset;

    trans.setIdentity();
    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(btQuaternion(rotation.GetXAxisValue(), rotation.GetYAxisValue(), rotation.GetZAxisValue(), rotation.w));
    m_sphere->calculateLocalInertia(m_mass, localInertia);

    m_motionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_sphere, localInertia);
    m_rigidbody = new btRigidBody(rbInfo);

    ObjectElements::Model model = ConstructSphere();

    //TODO add a "AddModel" that take a model as parametre
    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);

    Systems::PhysicsSystem::AddCollider(this);
}

Engine::Components::SphereCollider::SphereCollider(Objects::GameObject* p_gameObject, std::shared_ptr<SphereCollider> p_other) : ICollider{ p_gameObject, SPHERE_COLLIDER }
{
    
    //init data
    btTransform trans;
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    auto position = m_gameObject->GetTransform()->GetPosition();
    auto rotation = m_gameObject->GetTransform()->GetRotation();

    m_mass = p_other->m_mass;
    m_offset = p_other->m_offset;
    m_sphere = p_other->m_sphere;
    
    Vector3D offsetD = m_offset;
    Quaternion q = rotation * offsetD * Quaternion::Conjugate(rotation);
    offsetD = { q.GetXAxisValue(), q.GetYAxisValue(), q.GetZAxisValue() };
    Vector3D offset = offsetD;
    position += offset;

    trans.setIdentity();
    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(btQuaternion(rotation.GetXAxisValue(), rotation.GetYAxisValue(), rotation.GetZAxisValue(), rotation.w));
    m_sphere->calculateLocalInertia(m_mass, localInertia);

    m_motionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_sphere, localInertia);
    m_rigidbody = new btRigidBody(rbInfo);

    ObjectElements::Model model = ConstructSphere();

    //TODO add a "AddModel" that take a model as parametre
    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);

    Systems::PhysicsSystem::AddCollider(this);
}

Engine::Components::SphereCollider::~SphereCollider()
{
    if (m_rigidbody)
        Engine::Systems::PhysicsSystem::GetWorld()->removeRigidBody(m_rigidbody);

    if (m_model)
        Managers::ResourceManager::RemoveModel(m_model->GetID());

    delete m_sphere;
    delete m_motionState;
    delete m_rigidbody;
}

void Engine::Components::SphereCollider::Serialize(std::ostream& p_stream)
{
    p_stream << typeid(*this).name() << "\n{\n" <<
        "   m_mass " << m_mass << "\n" <<
        "   m_offset " << m_offset.x << " " << m_offset.y << " " << m_offset.z << "\n" <<
        "   m_scale " << m_scale.x << " " << m_scale.y << " " << m_scale.z << "\n" <<
        "   m_radius " << m_sphere->getRadius() << "\n" <<
        "}\n";
}

void Engine::Components::SphereCollider::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
{
    m_gameObject = p_gameObject;

    std::vector<std::string> words;

    for (int i = 0; i < p_block.size(); ++i)
    {
        std::stringstream stringStream(p_block[i]);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (words[0] == "m_mass")
        {
            m_mass = std::stof(words[1]);
        }
        else if (words[0] == "m_offset")
        {
            m_offset.x = std::stof(words[1]);
            m_offset.y = std::stof(words[2]);
            m_offset.z = std::stof(words[3]);
        }
        else if (words[0] == "m_scale")
        {
            m_scale.x = std::stof(words[1]);
            m_scale.y = std::stof(words[2]);
            m_scale.z = std::stof(words[3]);
        }
        else if (words[0] == "m_radius")
        {
            delete m_sphere;
            m_sphere = new btSphereShape(std::stof(words[1]));
        }

        words.clear();
    }

    //TODO: Memory leak here
    /*delete m_sphere;
    delete m_rigidbody;
    delete m_motionState;*/

    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    btTransform trans;
    auto position = m_gameObject->GetTransform()->GetPosition();
    auto rotation = m_gameObject->GetTransform()->GetRotation();

    Vector3D offsetD = m_offset;
    Quaternion q = rotation * offsetD * Quaternion::Conjugate(rotation);
    offsetD = { q.GetXAxisValue(), q.GetYAxisValue(), q.GetZAxisValue() };
    Vector3D offset = offsetD;
    position += offset;

    trans.setIdentity();
    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(btQuaternion(rotation.GetXAxisValue(), rotation.GetYAxisValue(), rotation.GetZAxisValue(), rotation.w));
    m_sphere->calculateLocalInertia(m_mass, localInertia);

    m_motionState = new btDefaultMotionState(trans);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_sphere, localInertia);
    m_rigidbody = new btRigidBody(rbInfo);

    ObjectElements::Model model = ConstructSphere();

    //TODO add a "AddModel" that take a model as parametre
    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);

}

void Engine::Components::SphereCollider::SetPositionOffset(GPM::Vector3F p_offset)
{
    m_offset = p_offset;

    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    btTransform trans;
    auto position = m_gameObject->GetTransform()->GetPosition();
    auto rotation = m_gameObject->GetTransform()->GetRotation();

    Vector3D offsetD = m_offset;
    Quaternion q = rotation * offsetD * Quaternion::Conjugate(rotation);
    offsetD = { q.GetXAxisValue(), q.GetYAxisValue(), q.GetZAxisValue() };
    Vector3D offset = offsetD;
    position += offset;

    trans.setIdentity();
    trans.setOrigin(btVector3(position.x, position.y, position.z));
    trans.setRotation(btQuaternion(rotation.GetXAxisValue(), rotation.GetYAxisValue(), rotation.GetZAxisValue(), rotation.w));

}

void Engine::Components::SphereCollider::SetMass(float p_mass)
{
    m_mass = p_mass;

    if (m_rigidbody)
    {
        Systems::PhysicsSystem::GetWorld()->removeRigidBody(m_rigidbody);
    }

    auto localInertia = btVector3(0.0f, 0.0f, 0.0f);

    if (m_sphere)
        m_sphere->calculateLocalInertia(m_mass, localInertia);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_sphere, localInertia);
    delete m_rigidbody;
    m_rigidbody = new btRigidBody(rbInfo);
    Systems::PhysicsSystem::GetWorld()->addRigidBody(m_rigidbody);
}

void Engine::Components::SphereCollider::SetRadius(float p_radius)
{
    btVector3 localInertia(0.0f, 0.0f, 0.0f);

    if (m_rigidbody)
    {
        delete m_sphere;
        Systems::PhysicsSystem::GetWorld()->removeRigidBody(m_rigidbody);
        m_sphere = new btSphereShape(p_radius);
        m_rigidbody->setCollisionShape(m_sphere);
        m_sphere->calculateLocalInertia(m_mass, localInertia);
        m_rigidbody->setMassProps(m_mass, localInertia);
        m_rigidbody->updateInertiaTensor();
        Systems::PhysicsSystem::GetWorld()->addRigidBody(m_rigidbody);
    }

    Managers::ResourceManager::RemoveModel(m_model->GetID());
    ObjectElements::Model model = ConstructSphere();

    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);
}

/*void Engine::Components::SphereCollider::SetScale(const GPM::Vector3F& p_scale)
{
    m_scale = p_scale;
    m_sphere->setLocalScaling(btVector3(m_scale.x, m_scale.y, m_scale.z));
    Systems::PhysicsSystem::GetWorld()->updateSingleAabb(m_rigidbody);

    Managers::ResourceManager::RemoveModel(m_model->GetID());
    ObjectElements::Model model = ConstructSphere();

    const int32_t id = Managers::ResourceManager::AddModel(model);
    m_model = Managers::ResourceManager::FindModel(id);
}*/

bool Engine::Components::SphereCollider::DeleteFromMemory()
{
    Managers::ResourceManager::RemoveModel(m_model->GetID());
    Systems::PhysicsSystem::RemoveCollider(GetID());
    return true;
}

void Engine::Components::SphereCollider::SetActive(bool p_active)
{
    m_isActive = p_active;

    if (!m_isActive)
    {
        m_rigidbody->forceActivationState(DISABLE_SIMULATION);
        m_rigidbody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
        Systems::PhysicsSystem::GetWorld()->removeCollisionObject(m_rigidbody);
    }
    else
    {
        m_rigidbody->forceActivationState(ACTIVE_TAG);
        m_rigidbody->setCollisionFlags(!btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }
}

Engine::ObjectElements::Model Engine::Components::SphereCollider::ConstructSphere()
{
    std::vector<Geometry::Vertex> vertices;
    std::vector<unsigned short>   indices;

    float radius = m_sphere->getRadius();

    for (int i = 0; i < 16; ++i)
    {
        float a = static_cast<float>(i);
        vertices.emplace_back(Geometry::Vertex{ Vector3F{ std::cosf(a / 2) * radius * m_scale.x, std::sinf(a / 2) * radius * m_scale.y, 0.0f }, 
            Vector2F{}, GPM::Vector3F{} });
        
        indices.emplace_back(i);
    }
    for (int i = 0; i < 16; ++i)
    {
        float a = static_cast<float>(i);
        vertices.emplace_back(Geometry::Vertex{ Vector3F{ 0.0f, std::sinf(a / 2) * radius * m_scale.y, std::cosf(a / 2) * radius * m_scale.z }, Vector2F{}, GPM::Vector3F{} });
        indices.emplace_back(i + 16);
    }

    for (int i = 0; i < 16; ++i)
    {
        float a = static_cast<float>(i);
        vertices.emplace_back(Geometry::Vertex{ Vector3F{ std::cosf(a / 2) * radius * m_scale.x, 0.0f, std::sinf(a / 2) * radius * m_scale.z }, Vector2F{}, GPM::Vector3F{} });
        indices.emplace_back(i + 32);
    }

    ObjectElements::Model model;

    auto mesh = std::make_shared<ObjectElements::Mesh>(vertices, indices);

    model.AddMesh(mesh);
    return model;
}
