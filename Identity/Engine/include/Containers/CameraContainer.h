#pragma once
#include <Export.h>
#include <map>
#include <Rendering/Camera.h>

namespace Engine::Containers
{
    class API_ENGINE CameraContainer
    {
    public:
        ~CameraContainer();
        CameraContainer(const CameraContainer&) = delete;
        CameraContainer(const CameraContainer&&) = delete;

        static int32_t AddCamera(Rendering::Camera& p_camera);

        static CameraContainer* GetInstance();
        static std::shared_ptr<Rendering::Camera> GetCamera(int32_t p_id);
        inline static std::map<int, std::shared_ptr<Rendering::Camera>>& GetAllCameras() { return GetInstance()->m_cameras; }
        inline static std::shared_ptr<Rendering::Camera> GetActiveCamera() { return GetInstance()->m_cameras.at(GetInstance()->m_activeCamera); }

        static void SetActiveCamera(std::shared_ptr<Rendering::Camera> p_camera);
        inline static void SetActiveCamera(int32_t p_id) { GetInstance()->m_activeCamera = p_id; }

    private:
        CameraContainer() = default;

        inline static CameraContainer* m_instance = nullptr;
        int32_t m_activeCamera = -1;
        std::map<int32_t, std::shared_ptr<Rendering::Camera>> m_cameras;
    };
}
