#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Rendering::Lights
{
    class API_ENGINE ILight: public Engine::Objects::IObject
    {
        virtual bool operator==(Rendering::Lights::ILight* p_other) = 0;
    };
}
