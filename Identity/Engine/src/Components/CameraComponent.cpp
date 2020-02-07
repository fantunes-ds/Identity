#include <stdafx.h>
#include <Components/CameraComponent.h>
#include <Containers/CameraContainer.h>

bool Engine::Components::CameraComponent::operator==(IComponent* p_other)
{
    if (m_camera == dynamic_cast<CameraComponent*>(p_other)->m_camera)
        return true;

    return false;
}

bool Engine::Components::CameraComponent::DeleteFromMemory()
{
    return Containers::CameraContainer::RemoveCamera(m_camera);
}

Engine::Components::CameraComponent::CameraComponent(const int p_width, const int p_height)
{
    Rendering::Camera camera(p_width, p_height);
    m_camera = Containers::CameraContainer::AddCamera(&camera);
}
