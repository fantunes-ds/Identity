#pragma once
#include <Export.h>
#include <vector>
#include <Components/IComponent.h>
#include <filesystem>
#include <irrKlang.h>

namespace Engine::Components
{
    class API_ENGINE Sound: public IComponent
    {
    public:

        Sound(Objects::GameObject* p_gameObject);
        Sound(Objects::GameObject* p_gameObject, const std::string& p_soundFile);
        Sound(Objects::GameObject* p_gameObject, std::shared_ptr<Sound> p_other);
        virtual ~Sound();
        Sound(const Sound&) = default;

        void PlaySound();
        void Pause();
        void Stop();

        void Serialize(std::ostream& p_stream) override;
        void Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block) override;
        bool DeleteFromMemory() override;
        bool operator==(IComponent* p_other) override;
        bool RemoveComponent() override;

        void SetActive(bool p_active) override;
        void SetPlayLooped(bool p_playLooped) { m_playLooped = p_playLooped; }
        void SetStartPaused(bool p_startPaused) { m_startPaused = p_startPaused; }
        void SetPlaySoundIn3D(bool p_3D) { m_playSoundIn3D = p_3D; }
        void SetIsPlaying(bool p_playing) { m_isPlaying = p_playing; }
        void SetMinDistance(float p_dist) { m_minDistance = p_dist; }
        void SetMaxDistance(float p_dist) { m_maxDistance = p_dist; }
        void SetVolume(float p_volume);
        void SetSoundFile(std::filesystem::path p_path);

        [[nodiscard]] bool GetPlayLooped() const { return m_playLooped; }
        [[nodiscard]] bool GetStartPaused() const { return m_startPaused; }
        [[nodiscard]] bool GetPlaySoundIn3D() const { return m_playSoundIn3D; }
        [[nodiscard]] bool IsPlaying() const { return m_isPlaying; }
        [[nodiscard]] float GetMinDistance() const { return m_minDistance; }
        [[nodiscard]] float GetMaxDistance() const { return m_maxDistance; }
        [[nodiscard]] float GetVolume() const { return m_volume; }
        [[nodiscard]] irrklang::ISound* GetISound() const { return m_sound; }
        [[nodiscard]] const std::string& GetFilePath() const { return m_soundFile; }

    private:
        bool m_playLooped = true;
        bool m_startPaused = false;
        bool m_playSoundIn3D = false;
        bool m_isPlaying = false;
        float m_minDistance = 1.0f;
        float m_maxDistance = 10.0f;
        float m_volume = 1.0f;
        irrklang::ISound* m_sound = nullptr;
        std::string m_soundFile;
    };
}
