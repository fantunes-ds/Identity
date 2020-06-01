#pragma once
#include <Export.h>
#include <Events/Event.h>

namespace Engine::Containers
{
    class CameraSystem;

    class API_ENGINE EventContainer
    {
    public:
        ~EventContainer();
        EventContainer(const EventContainer&) = delete;
        EventContainer(const EventContainer&&) = delete;

        static EventContainer* GetInstance();
        inline static std::map<std::string, Engine::Event>& GetAllEvents() { return GetInstance()->m_events; }
        static Engine::Event& GetEvent(const std::string& p_name);

        static void AddEvent(const std::string& p_name);

    private:
        EventContainer() = default;

        inline static EventContainer* m_instance = nullptr;
        std::map<std::string, Engine::Event> m_events;
    };
}
