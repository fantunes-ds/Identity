#include <stdafx.h>
#include <Managers/ResourceManager.h>

std::unique_ptr<Engine::Managers::ResourceManager>& Engine::Managers::ResourceManager::GetInstance()
{
    if (m_instance == nullptr)
        m_instance = std::make_unique<ResourceManager>();

    return m_instance;
}
