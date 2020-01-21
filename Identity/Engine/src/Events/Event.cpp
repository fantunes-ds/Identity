#include <stdafx.h>
#include <Events/Event.h>

void Engine::Event::AddListener(IEventCallback* p_action)
{
    //check if delegate is already added to vector
    auto position = find(m_actions.begin(), m_actions.end(), p_action);

    if (position != m_actions.end())
    {
        return;
    }

    m_actions.push_back(p_action);
}

void Engine::Event::Fire()
{
    for (IEventCallback* action: m_actions)
    {
        (*action)();
    }
}

