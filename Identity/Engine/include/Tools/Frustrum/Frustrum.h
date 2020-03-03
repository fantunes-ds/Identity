#pragma once
#include <Export.h>
#include <DirectXMath.h>

namespace Engine::Tools::Frustrum
{
    class API_ENGINE Frustrum
    {
    public:
        Frustrum() = default;
        Frustrum(const Frustrum& p_other) = default;
        ~Frustrum() = default;

        void ConstructFrustrum(float p_screenDepth, GPM::Matrix4F& p_projection, GPM::Matrix4F& p_viewMatrix);


    };
}
