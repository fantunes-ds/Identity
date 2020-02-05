#include <stdafx.h>
#include <Components/ModelComponent.h>

bool Engine::Components::ModelComponent::IsWellInitialized()
{
    if (m_model >= 0)
        return true;

    return false;
}


