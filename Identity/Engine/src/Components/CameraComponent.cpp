#include <stdafx.h>
#include <Components/CameraComponent.h>

bool Engine::Components::CameraComponent::IsWellInitialized()
{
    return true;
}

bool Engine::Components::CameraComponent::operator==(IComponent* p_other)
{
    return false;
}

