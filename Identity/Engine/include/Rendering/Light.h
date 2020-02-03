#pragma once

namespace Engine::Rendering
{
    struct Light
    {
        Vector4F position;
        Vector4F ambient;
        Vector4F diffuse;
        Vector4F specular;
        Vector4F direction;
        Vector4F color;
        float shininess;
    };
}
