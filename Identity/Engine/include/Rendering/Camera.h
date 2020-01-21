#pragma once
#include <Export.h>
#include <GPM/GPM.h>

namespace Engine::Rendering
{
    class API_ENGINE Camera
    {
    public:
        Camera();
        ~Camera() = default;

        void SetProjectionValues();

    private:
        void UpdateViewMatrix();
        GPM::Vector3D m_position;
    };
}
