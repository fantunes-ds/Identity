#pragma once
#include <Export.h>
#include <Events/Event.h>
#include <debugapi.h>

namespace Engine
{
    class API_ENGINE EventReceiver
    {
    public:
        void Handler(int p_value)
        {
            OutputDebugString("called handler");
            x = "hello event is working";
        }

        void hookEvent(Event* p_event)
        {
            __hook(&Event::DoThing, p_event, &EventReceiver::Handler);
        }

        std::string x = "didnt receive event\n";
    };
}
