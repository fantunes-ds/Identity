#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Rendering::Lights
{
    class API_ENGINE ILight: public Engine::Objects::IObject
    {
    public:
        virtual bool operator==(Rendering::Lights::ILight* p_other) = 0;
        virtual void Serialize(std::ostream& p_stream) = 0;
        virtual void Deserialize(std::istream& p_stream) = 0;
    };
}
