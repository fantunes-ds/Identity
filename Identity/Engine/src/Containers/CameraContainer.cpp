#include <stdafx.h>
#include <Containers/CameraContainer.h>
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

int32_t Engine::Containers::CameraContainer::AddCamera(Rendering::Camera& p_camera)
{
    GetInstance()->m_cameras.insert_or_assign(p_camera.GetID(), std::make_shared<Rendering::Camera>(p_camera));
    return p_camera.GetID();
}

std::shared_ptr<Engine::Rendering::Camera> Engine::Containers::CameraContainer::GetCamera(int32_t p_id)
{
    return GetInstance()->m_cameras.at(p_id);
}

void Engine::Containers::CameraContainer::SetActiveCamera(std::shared_ptr<Rendering::Camera> p_camera)
{
    
}

