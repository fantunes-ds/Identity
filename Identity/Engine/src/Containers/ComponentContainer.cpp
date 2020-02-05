#include <stdafx.h>
#include <typeinfo>
#include <windows.h>
#include <Containers/ComponentContainer.h>
#include <Tools/IDCounter.h>

Engine::Containers::ComponentContainer::~ComponentContainer()
{
    delete m_instance;
}

int32_t Engine::Containers::ComponentContainer::AddComponent(Components::IComponent* p_component)
{
    for (auto& component : GetInstance()->m_components)
    {
        if (typeid(*component.second) == typeid(*p_component))
        {          
            if (*component.second == p_component)
            {
                std::string type = typeid(*p_component).name();
                const std::string error("ComponentContainer::AddComponent<" + type + ">(Components::IComponent* p_component): Failed to add Component because it already exists");
                MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
                return -1;
            }
        }
    }

    int32_t id = Tools::IDCounter::GetNewID();
    GetInstance()->m_components.insert_or_assign(id, std::shared_ptr<Engine::Components::IComponent>(p_component));
    return id;
}

Engine::Containers::ComponentContainer* Engine::Containers::ComponentContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new ComponentContainer();
    }

    return m_instance;
}

std::shared_ptr<Engine::Components::IComponent> Engine::Containers::ComponentContainer::FindComponent(int32_t p_id)
{
    return GetInstance()->GetAllComponents().at(p_id);
}
