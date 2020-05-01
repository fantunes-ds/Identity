#include <stdafx.h>
#include <Systems/CameraSystem.h>
#include <windows.h>
#include <Rendering/Renderer.h>
#include <Managers/SceneManager.h>

Engine::Containers::CameraSystem::~CameraSystem()
{
    delete m_instance;
}

Engine::Containers::CameraSystem* Engine::Containers::CameraSystem::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new CameraSystem();
    }

    return m_instance;
}

int32_t Engine::Containers::CameraSystem::AddCamera(std::shared_ptr<Components::Camera> p_camera)
{
    int32_t id = p_camera->GetID();

    GetInstance()->m_cameras.insert_or_assign(id, std::shared_ptr<Components::Camera>(p_camera));

    return id;
}

bool Engine::Containers::CameraSystem::RemoveCamera(int32_t p_id)
{
    size_t sizeBefore = GetInstance()->m_cameras.size();
    GetInstance()->m_cameras.erase(p_id);
    size_t sizeAfter = GetInstance()->m_cameras.size();

    if (sizeBefore == sizeAfter)
        return false;

    return true;
}

void Engine::Containers::CameraSystem::Update(const float p_deltaTime)
{
    GetInstance()->IUpdate(p_deltaTime);
}

void Engine::Containers::CameraSystem::IUpdate(const float p_deltaTime, bool p_isEditor)
{
    for (std::shared_ptr<Engine::Components::Camera> camera : GetCameras())
    {
        if (camera->IsActive())
        {
            int width, height;
            Rendering::Renderer::GetInstance()->GetResolution(width, height);
            camera->UpdateCamera(p_deltaTime, width, height);
        }
    }
}

std::shared_ptr<Engine::Components::Camera> Engine::Containers::CameraSystem::GetCamera(int32_t p_id)
{
    if (GetInstance()->m_cameras.find(p_id) != GetInstance()->m_cameras.end())
        return GetInstance()->m_cameras.at(p_id);

    const std::string error("CameraSystem::GetCamera(int32_t p_id): Didn't find Camera with ID " + std::to_string(p_id) + "\n");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<Engine::Components::Camera>> Engine::Containers::CameraSystem::GetCameras()
{
    std::vector<std::shared_ptr<Engine::Components::Camera>> cameras;

    for (const auto& camera : GetAllCameras())
    {
        cameras.push_back(camera.second);
    }

    return cameras;
}
