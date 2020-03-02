#include <stdafx.h>
#include <Containers/CameraContainer.h>
#include <Containers/EventContainer.h>
#include <Tools/IDCounter.h>
#include <windows.h>

Engine::Containers::CameraContainer::~CameraContainer()
{
    delete m_instance;
}

Engine::Containers::CameraContainer* Engine::Containers::CameraContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new CameraContainer();
    }

    return m_instance;
}

int32_t Engine::Containers::CameraContainer::AddCamera(std::shared_ptr<Rendering::Camera> p_camera)
{
    int32_t id = p_camera->GetID();

    GetInstance()->m_cameras.insert_or_assign(id, std::shared_ptr<Rendering::Camera>(p_camera));

    return id;
}

bool Engine::Containers::CameraContainer::RemoveCamera(int32_t p_id)
{
    size_t sizeBefore = GetInstance()->m_cameras.size();
    GetInstance()->m_cameras.erase(p_id);
    size_t sizeAfter = GetInstance()->m_cameras.size();

    if (sizeBefore == sizeAfter)
        return false;

    return true;
}

std::shared_ptr<Engine::Rendering::Camera> Engine::Containers::CameraContainer::GetCamera(int32_t p_id)
{

    if (GetInstance()->m_cameras.find(p_id) != GetInstance()->m_cameras.end())
        return GetInstance()->m_cameras.at(p_id);

    const std::string error("CameraContainer::GetCamera(int32_t p_id): Didn't find Camera with ID " + std::to_string(p_id) + "\n");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
    return nullptr;
}

/*void Engine::Containers::CameraContainer::SetActiveCamera(std::shared_ptr<Rendering::Camera> p_camera)
{

    GetInstance()->m_activeCamera = p_camera->GetID();
    EventContainer::GetEvent("ChangeActiveCamera").Fire();
}

void Engine::Containers::CameraContainer::SetActiveCamera(int32_t p_id)
{
    GetInstance()->m_activeCamera = p_id;
    EventContainer::GetEvent("ChangeActiveCamera").Fire();
}*/

