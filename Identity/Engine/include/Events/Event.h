#pragma once
#include <Export.h>
#include <Events/IEventCallback.h>

namespace Engine
{
    class API_ENGINE Event
    {
    public:
        Event() = default;
        ~Event() = default;

        void AddListener(IEventCallback* p_action);
        void RemoveListener(IEventCallback* p_action);
        void Fire();

    private:
        std::vector<IEventCallback*> m_actions;
    };
}
