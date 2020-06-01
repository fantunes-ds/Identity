#include <stdafx.h>

#include <Events/Event.h>

using namespace Engine;

void Event::Fire()
{
    for (auto action : m_actions)
    {
        (*action.second)();
    }
}
