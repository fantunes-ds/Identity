#pragma once
#include <Export.h>
#include <GPM/GPM.h>

namespace Engine::Geometry
{
    struct API_ENGINE Vertex
    {
        Vector3F m_position{};
        Vector2F m_textCoords{};
        Vector3F m_normal{};
    };
}
