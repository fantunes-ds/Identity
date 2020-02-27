#include <stdafx.h>
#include <Components/CameraComponent.h>
#include <Containers/CameraContainer.h>
#include <Objects\GameObject.h>

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

Engine::Components::CameraComponent::CameraComponent(Objects::GameObject* p_gameObject, const int p_width, const int p_height): IComponent{ p_gameObject }
{
    auto camera = std::make_shared<Rendering::Camera>(p_width, p_height);
    m_camera = Containers::CameraContainer::AddCamera(camera);
}
