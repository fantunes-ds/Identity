#include <stdafx.h>

#include <Components/Transform.h>
#include <Systems/TransformSystem.h>

Engine::Components::Transform::Transform(Objects::GameObject* p_gameObject) : IComponent{p_gameObject, TRANSFORM}, m_position{Vector3F::zero},
m_forward{Vector3F::forward}, m_right{Vector3F::right}, m_up{Vector3F::up}, m_scale{Vector3F::one},
m_rotation{ Quaternion{0.0, 0.0, 0.0, 1.0} }
{
    Systems::TransformSystem::AddTransform(std::shared_ptr<Transform>(this));
}

Engine::Components::Transform::Transform(Objects::GameObject* p_gameObject, const Transform& p_other) : IComponent{p_gameObject, TRANSFORM},
m_parent{p_other.m_parent}, m_position{p_other.m_position}, m_forward{p_other.m_forward}, m_right{p_other.m_right},
m_up{p_other.m_up}, m_scale{p_other.m_scale}, m_rotation{ p_other.m_rotation } {}


Engine::Components::Transform::Transform() : IComponent{nullptr, TRANSFORM},
                                             m_position{Vector3F::zero}, m_forward{Vector3F::forward}, m_right{Vector3F::right},
                                             m_up{Vector3F::up}, m_scale{Vector3F::one}, m_rotation{Quaternion{0.0, 0.0, 0.0, 1.0}} {}

Engine::Components::Transform::Transform(const std::string& p_name) : IComponent{nullptr, TRANSFORM}, m_position{Vector3F::zero},
                                                                      m_forward{Vector3F::forward}, m_right{Vector3F::right},
                                                                      m_up{Vector3F::up}, m_scale{Vector3F::one},
                                                                      m_rotation{Quaternion{0.0, 0.0, 0.0, 1.0}}
{
    SetName(p_name);
}

void Engine::Components::Transform::CopyFrom(std::shared_ptr<Transform> p_other)
{
    m_position = p_other->m_position;
    m_forward  = p_other->m_forward;
    m_right    = p_other->m_right;
    m_up       = p_other->m_up;
    m_scale    = p_other->m_scale;
    m_rotation = p_other->m_rotation;

    // needUpdate = true;
    // needAxesUpdate = true;
}

bool Engine::Components::Transform::operator==(IComponent* p_other)
{
    return true;
}

bool Engine::Components::Transform::DeleteFromMemory()
{
    return true;
}

void Engine::Components::Transform::Serialize(std::ostream& p_stream)
{
    p_stream << typeid(*this).name() << " " << std::to_string(m_id) << "\n{\n" <<
        "   m_parent " << m_parent << "\n" <<
        "   m_position " << m_position << "\n" <<
        "   m_forward " << m_forward << "\n" <<
        "   m_right " << m_right << "\n" <<
        "   m_up " << m_up << "\n" <<
        "   m_scale " << m_scale << "\n" <<
        "   m_rotation " << m_rotation << "\n" <<
        "   m_worldTransform " << m_worldTransform << "\n" <<
        "   m_localTransform " << m_localTransform << "\n" <<
        "}\n";
}

void Engine::Components::Transform::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
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

        if (words[0] == "m_parent")
        {
            m_parent = std::stoi(words[1]);
        }
        else if (words[0] == "m_position")
        {
            m_position.x = std::stof(words[1]);
            m_position.y = std::stof(words[2]);
            m_position.z = std::stof(words[3]);
        }
        else if (words[0] == "m_forward")
        {
            m_forward.x = std::stof(words[1]);
            m_forward.y = std::stof(words[2]);
            m_forward.z = std::stof(words[3]);
        }
        else if (words[0] == "m_right")
        {
            m_right.x = std::stof(words[1]);
            m_right.y= std::stof(words[2]);
            m_right.z = std::stof(words[3]);
        }
        else if (words[0] == "m_up")
        {
            m_up.x = std::stof(words[1]);
            m_up.y = std::stof(words[1]);
            m_up.z = std::stof(words[1]);
        }
        else if (words[0] == "m_scale")
        {
            m_scale.x = std::stof(words[1]);
            m_scale.y = std::stof(words[2]);
            m_scale.z = std::stof(words[3]);
        }
        else if (words[0] == "m_rotation")
        {
            m_rotation.SetXAxisValue(std::stof(words[1]));
            m_rotation.SetYAxisValue(std::stof(words[2]));
            m_rotation.SetZAxisValue(std::stof(words[3]));
            m_rotation.SetRealValue(std::stof(words[4]));
        }
        else if (words[0] == "m_worldTransform")
        {
            m_worldTransform = Matrix4F
            (
                std::stof(words[1]), std::stof(words[2]), std::stof(words[3]), std::stof(words[4]),
                std::stof(words[5]), std::stof(words[6]), std::stof(words[7]), std::stof(words[8]),
                std::stof(words[9]), std::stof(words[10]), std::stof(words[11]), std::stof(words[12]),
                std::stof(words[13]), std::stof(words[14]), std::stof(words[15]), std::stof(words[16])
            );
        }
        else if (words[0] == "m_localTransform")
        {
            m_localTransform = Matrix4F
            (
                std::stof(words[1]), std::stof(words[2]), std::stof(words[3]), std::stof(words[4]),
                std::stof(words[5]), std::stof(words[6]), std::stof(words[7]), std::stof(words[8]),
                std::stof(words[9]), std::stof(words[10]), std::stof(words[11]), std::stof(words[12]),
                std::stof(words[13]), std::stof(words[14]), std::stof(words[15]), std::stof(words[16])
            );
        }

        words.clear();
    }

}

void Engine::Components::Transform::SetActive(bool p_active)
{
    m_isActive = p_active;
    // std::string message("Transform components cannot be deactivated.\n");
    // MessageBox(nullptr, message.c_str(), "Error", MB_ICONWARNING | MB_OK);
}

void Engine::Components::Transform::Translate(const Vector3F& p_vector)
{
    m_position += p_vector;
    needUpdate = true;
}

void Engine::Components::Transform::RotateWithEulerAngles(const Vector3F& p_euler)
{
    //TODO: check if it properly accepts angles > 360
    Quaternion quat;
    quat.MakeFromEuler(Vector3F{p_euler.x, p_euler.y, p_euler.z});
    m_rotation *= quat;
    needAxesUpdate = true;
    needUpdate     = true;
}

void Engine::Components::Transform::Scale(const Vector3F& p_scale)
{
    m_scale *= p_scale;
    needUpdate = true;
}

void Engine::Components::Transform::UpdateWorldTransformMatrix()
{
    m_worldTransform = Matrix4F::CreateTransformation(m_position,
                                                      m_rotation,
                                                      m_scale);
    needUpdate = false;
}

Vector3F Engine::Components::Transform::GetEuler() const
{
    return m_rotation.ToEuler();
}

std::shared_ptr<Engine::Components::Transform> Engine::Components::Transform::GetParent() const
{
    return Systems::TransformSystem::FindTransform(m_parent);
}

void Engine::Components::Transform::CalculateAxes()
{
    Quaternion quatf = (m_rotation * Vector3F::forward * m_rotation.Conjugate());
    Quaternion quatr = (m_rotation * Vector3F::right * m_rotation.Conjugate());
    Quaternion quatu = (m_rotation * Vector3F::up * m_rotation.Conjugate());

    Vector3F vec3f = quatf.GetRotationAxis();
    Vector3F vec3r = quatr.GetRotationAxis();
    Vector3F vec3u = quatu.GetRotationAxis();

    m_forward = Vector3F{-vec3f.x, -vec3f.y, vec3f.z};
    m_right   = Vector3F{vec3r.x, vec3r.y, vec3r.z};
    m_up      = Vector3F{vec3u.x, vec3u.y, -vec3u.z};

    m_right = Vector3F::Cross(m_up, m_forward);
    m_up    = Vector3F::Cross(m_right, m_forward);

    needAxesUpdate = false;
}
