#include <stdafx.h>
#include <Events/Event.h>

void Engine::Event::Fire()
{
    for (auto action : m_actions)
    {
        (*action.second)();
    }
}