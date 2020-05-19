#pragma once
#include <Export.h>
#include <Systems/ISystem.h>
#include <irrKlang.h>

namespace Engine::Systems
{
    class API_ENGINE SoundSystem : Systems::ISystem
    {
    public:
        ~SoundSystem();

        static void Update(const float p_deltaTime);

        static SoundSystem* GetInstance();
        static irrklang::ISoundEngine* GetSoundEngine() { return GetInstance()->m_soundEngine; }

    private:
        SoundSystem();
        void IUpdate(const float p_deltaTime, bool p_isEditor = false) override;

        static inline SoundSystem* m_instance;
        irrklang::ISoundEngine* m_soundEngine;
    };
}
