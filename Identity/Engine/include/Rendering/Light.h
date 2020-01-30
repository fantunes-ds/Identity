#pragma once

namespace Engine::Rendering
{
    struct Light
    {
        Vector3F position;
        Vector3F ambient;
        Vector3F diffuse;
        Vector3F specular;
        Vector3F direction;
    };
}
