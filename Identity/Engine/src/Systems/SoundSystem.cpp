#include <stdafx.h>
#include <Systems/SoundSystem.h>

#include <Systems/CameraSystem.h>
#include <Systems/RenderSystem.h>

#include <Containers/GameObjectContainer.h>
#include <Managers/SceneManager.h>
#include <Scene/Scene.h>
#include <Components/Transform.h>
#include <Components/Sound.h>

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
    auto& activeCamPos = Systems::RenderSystem::GetActiveCamera()->GetPosition();
    auto targetDir = Systems::RenderSystem::GetActiveCamera()->GetGameObject()->GetTransform()->GetForward() - activeCamPos;

    m_soundEngine->setListenerPosition(irrklang::vec3df(activeCamPos.x, activeCamPos.y, activeCamPos.z),
        irrklang::vec3df(targetDir.x, targetDir.y, targetDir.z));

    for (auto& sound : m_sounds)
    {
        if (sound.second->IsActive() && sound.second->GetISound())
        {
            if (sound.second->GetISound()->isFinished())
            {
                sound.second->SetIsPlaying(false);
            }

            sound.second->PlaySound();
        }
    }
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

void Engine::Systems::SoundSystem::StopAllSounds()
{
    for (auto sound : GetInstance()->m_sounds)
    {
        sound.second->Stop();
    }
}

void Engine::Systems::SoundSystem::AddSound(std::shared_ptr<Components::Sound> p_sound)
{
    GetInstance()->m_sounds.insert_or_assign(p_sound->GetID(), p_sound);
}

void Engine::Systems::SoundSystem::RemoveSound(std::shared_ptr<Components::Sound> p_sound)
{
    if (p_sound == nullptr)
        return;

    GetInstance()->m_sounds.erase(p_sound->GetID());
}

void Engine::Systems::SoundSystem::RemoveSound(int32_t p_id)
{
    if (p_id < 0)
        return;

    GetInstance()->m_sounds.erase(p_id);
}

Engine::Systems::SoundSystem::SoundSystem()
{
    m_soundEngine = irrklang::createIrrKlangDevice();
}
