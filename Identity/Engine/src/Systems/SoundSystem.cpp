#include <stdafx.h>
#include <Systems/SoundSystem.h>

#include <Systems/CameraSystem.h>
#include <Systems/RenderSystem.h>

#include "Containers/GameObjectContainer.h"
#include "Managers/SceneManager.h"
#include <Scene/Scene.h>
#include <Components/Transform.h>

Engine::Systems::SoundSystem* Engine::Systems::SoundSystem::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SoundSystem();
    }

    return m_instance;
}

void Engine::Systems::SoundSystem::IUpdate(const float p_deltaTime, bool p_isEditor)
{
    auto activeCamPos = Systems::RenderSystem::GetActiveCamera()->GetPosition();
    auto targetDir = Systems::RenderSystem::GetActiveCamera()->GetGameObject()->GetTransform()->GetForward() - activeCamPos;

    m_soundEngine->setListenerPosition(irrklang::vec3df(activeCamPos.x, activeCamPos.y, activeCamPos.z), 
        irrklang::vec3df(targetDir.x, targetDir.y, targetDir.z));
}

Engine::Systems::SoundSystem::~SoundSystem()
{
    delete m_soundEngine;
    delete m_instance;
}

void Engine::Systems::SoundSystem::Update(const float p_deltaTime)
{
    GetInstance()->IUpdate(p_deltaTime);
}

Engine::Systems::SoundSystem::SoundSystem()
{
    m_soundEngine = irrklang::createIrrKlangDevice();
}
