#include <stdafx.h>
#include <Tools/Frustrum/Frustrum.h>

void Engine::Tools::Frustrum::Frustrum::ConstructFrustrum(float p_screenDepth, GPM::Matrix4F& p_projection, GPM::Matrix4F& p_viewMatrix)
{
    float zMinimum;
    float r;
    GPM::Matrix4F matrix;

    zMinimum = -p_projection.
}
