#include <stdafx.h>

#include <Objects/GameObject.h>

#include <Containers/ComponentContainer.h>
#include <Containers/GameObjectContainer.h>
#include <Managers/ResourceManager.h>
#include <Managers/SceneManager.h>
#include <Systems/TransformSystem.h>
#include <Systems/LightSystem.h>

#include <Scene/Scene.h>

#include <Components/ModelComponent.h>
#include <Components/BoxCollider.h>
#include <Components/Sound.h>
#include <Components/Light.h>

#include <Components/SphereCollider.h>

using namespace Engine::Objects;

GameObject::GameObject()
{
    m_transform = AddComponent<Components::Transform>();
    Systems::TransformSystem::FindTransform(m_transform)->SetGameObject(this);
    Containers::GameObjectContainer::AddGameObject(std::shared_ptr<GameObject>(this));
}

GameObject::GameObject(const std::string& p_name)
{
    m_transform = Systems::TransformSystem::AddTransform(std::make_shared<Components::Transform>(p_name));
    Containers::GameObjectContainer::AddGameObject(std::shared_ptr<GameObject>(this));
    SetName(p_name);
    auto trm = Systems::TransformSystem::GetTransform(m_transform);
    Containers::ComponentContainer::AddComponent(trm.get());
}

void GameObject::DeleteFromMemory()
{
    for (auto& component : m_components)
    {
        Containers::ComponentContainer::RemoveComponent(component);
    }

    Containers::GameObjectContainer::RemoveGameObject(GetID());

    Managers::SceneManager::GetActiveScene()->RemoveGameObject(GetID());
}

void GameObject::Serialize(std::ostream& p_stream)
{
    p_stream << "\nGAMEOBJECT\n" << m_name << "\n" <<
        "m_isActive " << m_isActive << "\n";

    GetTransform()->Serialize(p_stream);

    for (auto component: GetAllComponents())
    {
        Containers::ComponentContainer::FindComponent(component)->Serialize(p_stream);
    }

    p_stream << ";\n";
}

void GameObject::Deserialize(std::vector<std::string>& p_strings)
{
    std::vector<std::string> words;
    std::vector<std::string> componentBlock;

    for (int i = 0; i < p_strings.size(); ++i)
    {
        std::stringstream stringStream(p_strings[i]);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (i == 1)
        {
            m_name = words[0];
        }
        else if (words[0] == "m_isActive")
        {
            m_isActive = std::stoi(words[1]);
        }
        else if (words[0] == "class")
        {
            while (p_strings[i] != "}")
            {
                componentBlock.push_back(p_strings[i]);
                ++i;
            }

            if (words[1] == "Engine::Components::Transform")
            {
                Containers::ComponentContainer::FindComponent(m_transform)->Deserialize(this, componentBlock);
            }
            else if (words[1] == "Engine::Components::BoxCollider")
            {
                int id = AddComponent<Components::BoxCollider>(componentBlock);
            }
            else if (words[1] == "Engine::Components::SphereCollider")
            {
                int id = AddComponent<Components::SphereCollider>(componentBlock);
            }
            else if (words[1] == "Engine::Rendering::Lights::DirectionalLight")
            {
                int compID = AddComponent<Components::Light>();
                Systems::LightSystem::GetLight(compID)->Deserialize(this, componentBlock);
            }
            else if (words[1] == "Engine::Components::ModelComponent")
            {
                int compID = AddComponent<Components::ModelComponent>();
                Containers::ComponentContainer::FindComponent(compID)->Deserialize(this, componentBlock);
            }
            else if (words[1] == "Engine::Components::Sound")
            {
                int compID = AddComponent<Components::Sound>();
                Containers::ComponentContainer::FindComponent(compID)->Deserialize(this, componentBlock);
            }

            componentBlock.clear();
        }

        words.clear();
    }
}

std::shared_ptr<Engine::Components::Transform> GameObject::GetTransform() const
{
    return Systems::TransformSystem::GetTransform(m_transform);
}

std::shared_ptr<Engine::ObjectElements::Model> GameObject::GetModel() const
{
    if (this == nullptr)
        return nullptr;

    for (auto& component : m_components)
    {
        if (Components::ModelComponent* modelComp = dynamic_cast<Components::ModelComponent*>(&*Containers::ComponentContainer::FindComponent(component)))
            return Managers::ResourceManager::FindModel(modelComp->GetModel());
    }

    return nullptr;
}

inline void GameObject::SetActive(bool p_active)
{
    for (auto& component : GetAllComponents())
    {
        Containers::ComponentContainer::FindComponent(component)->SetActive(p_active);
    }

    m_isActive = p_active;
}

bool GameObject::operator==(GameObject& p_other) const
{
    if (m_transform == p_other.m_transform && m_components == p_other.m_components)
        return true;

    return false;
}

void GameObject::SetParentObject(std::shared_ptr<GameObject> p_parent)
{
    p_parent->GetSceneNode()->AddChild(m_rootNode);
}

bool GameObject::RemoveComponent(int32_t p_id)
{
    for (size_t i = 0; i < m_components.size(); ++i)
    {
        if (m_components[i] == p_id)
        {
            m_components.erase(m_components.begin() + i);
            Containers::ComponentContainer::RemoveComponent(p_id);
            return true;
        }
    }

    return false;
}
