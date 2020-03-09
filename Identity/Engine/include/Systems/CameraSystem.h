#pragma once
#include <Export.h>
#include <map>
#include <Systems/ISystem.h>
#include <Components/Camera.h>
#include <Containers/EventContainer.h>

namespace Engine::Containers
{
    class API_ENGINE CameraSystem : Systems::ISystem
    {
    public:
        ~CameraSystem();
        CameraSystem(const CameraSystem&) = delete;
        CameraSystem(const CameraSystem&&) = delete;

        static int32_t AddCamera(std::shared_ptr<Components::Camera> p_camera);
        static bool RemoveCamera(int32_t p_id);

        static void Update(float p_deltaTime);
        void IUpdate(float p_deltaTime) override;

        static CameraSystem* GetInstance();
        static std::shared_ptr<Components::Camera> GetCamera(int32_t p_id);
        static std::vector<std::shared_ptr<Engine::Components::Camera>> GetCameras();
        inline static std::map<int, std::shared_ptr<Components::Camera>>& GetAllCameras() { return GetInstance()->m_cameras; }

    private:
        CameraSystem() = default;

        inline static CameraSystem* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Components::Camera>> m_cameras;
    };
}
