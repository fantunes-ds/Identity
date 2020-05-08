#pragma once
#include <Export.h>
#include <iostream>

namespace Engine::Serialize
{
    class API_ENGINE ISerializeable
    {
    public:
        static void Serialize(std::ostream& p_stream)
        {

        }

        static ISerializeable* Unserialize(std::istream& p_stream);
    };
}
