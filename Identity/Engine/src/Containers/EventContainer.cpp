#include <stdafx.h>
#include <Containers/EventContainer.h>

Engine::Containers::EventContainer::~EventContainer()
{
    delete m_instance;
}

Engine::Containers::EventContainer* Engine::Containers::EventContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new EventContainer();
    }

    return m_instance;
}

void Engine::Containers::EventContainer::AddEvent(const std::string& p_name)
{
    GetInstance()->m_events.insert_or_assign(p_name, Event{});
}
