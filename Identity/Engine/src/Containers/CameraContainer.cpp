#include <stdafx.h>
#include <Containers/CameraContainer.h>
#include <Containers/EventContainer.h>
#include <Tools/IDCounter.h>

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

int32_t Engine::Containers::CameraContainer::AddCamera(Rendering::Camera* p_camera)
{
    int32_t id = p_camera->GetID();

    if (GetInstance()->m_cameras.insert_or_assign(id, std::make_shared<Rendering::Camera>(*p_camera)).second)
    {
        std::cout << "ble";
    }

    if (GetInstance()->m_cameras.size() == 1)
        SetActiveCamera(id);

    return id;
}

std::shared_ptr<Engine::Rendering::Camera> Engine::Containers::CameraContainer::GetCamera(int32_t p_id)
{
    return GetInstance()->m_cameras.at(p_id);
}

void Engine::Containers::CameraContainer::SetActiveCamera(std::shared_ptr<Rendering::Camera> p_camera)
{
    GetInstance()->m_activeCamera = p_camera->GetID();
    EventContainer::GetEvent("ChangeActiveCamera").Fire();
}

void Engine::Containers::CameraContainer::SetActiveCamera(int32_t p_id)
{
    GetInstance()->m_activeCamera = p_id;
    EventContainer::GetEvent("ChangeActiveCamera").Fire();
}

