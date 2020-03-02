#pragma once
#include <Export.h>
#include <map>
#include <Rendering/Camera.h>
#include "EventContainer.h"

namespace Engine::Containers
{
    class API_ENGINE CameraContainer
    {
    public:
        ~CameraContainer();
        CameraContainer(const CameraContainer&) = delete;
        CameraContainer(const CameraContainer&&) = delete;

        static int32_t AddCamera(std::shared_ptr<Rendering::Camera> p_camera);
        static bool RemoveCamera(int32_t p_id);

        static CameraContainer* GetInstance();
        static std::shared_ptr<Rendering::Camera> GetCamera(int32_t p_id);
        inline static std::map<int, std::shared_ptr<Rendering::Camera>>& GetAllCameras() { return GetInstance()->m_cameras; }

    private:
        CameraContainer() = default;

        inline static CameraContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Rendering::Camera>> m_cameras;
    };
}
