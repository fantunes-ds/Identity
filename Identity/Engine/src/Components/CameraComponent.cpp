#include <stdafx.h>
#include <Components/CameraComponent.h>
#include <Containers/CameraContainer.h>

bool Engine::Components::CameraComponent::IsWellInitialized()
{
    return true;
}

bool Engine::Components::CameraComponent::operator==(IComponent* p_other)
{
    return false;
}

Engine::Components::CameraComponent::CameraComponent(const int p_width, const int p_height)
{
    Rendering::Camera camera(p_width, p_height);
    m_camera = Containers::CameraContainer::AddCamera(&camera);
}
