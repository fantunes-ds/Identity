#pragma once
#include <Export.h>
#include <iostream>

namespace Engine::Serialize
{
    class API_ENGINE ISerializeable
    {
    public:
        virtual void Serialize(std::ostream& p_stream) = 0;
        virtual ISerializeable* Unserialize(std::istream& p_stream) = 0;
    };
}
