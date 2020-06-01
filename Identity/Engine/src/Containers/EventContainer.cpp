#include <stdafx.h>

#include <windows.h>

#include <Containers/EventContainer.h>

using namespace Engine::Containers;

EventContainer::~EventContainer()
{
    delete m_instance;
}

EventContainer* EventContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new EventContainer();
    }

    return m_instance;
}

Engine::Event& EventContainer::GetEvent(const std::string& p_name)
{
    if (GetInstance()->m_events.find(p_name) == GetInstance()->m_events.end())
    {
        const std::string error("EventContainer::GetEvent(const std::string& p_name): could not find event with name " + p_name + "\n");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        //return;
    }

     return GetInstance()->m_events.at(p_name); 
}

void EventContainer::AddEvent(const std::string& p_name)
{
    GetInstance()->m_events.insert_or_assign(p_name, Event{});
}
