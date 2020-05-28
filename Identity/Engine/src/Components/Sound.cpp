#include <stdafx.h>
#include <Components/Sound.h>
#include <Systems/SoundSystem.h>
#include <Objects/GameObject.h>

Engine::Components::Sound::Sound(Objects::GameObject* p_gameObject) : IComponent{ p_gameObject, SOUND }
{
    Systems::SoundSystem::AddSound(std::shared_ptr<Components::Sound>(this));
}

Engine::Components::Sound::Sound(Objects::GameObject* p_gameObject, const std::string& p_soundFile) : IComponent{ p_gameObject, SOUND }, m_soundFile{ p_soundFile }
{
    Systems::SoundSystem::AddSound(std::shared_ptr<Components::Sound>(this));
}

Engine::Components::Sound::Sound(Objects::GameObject* p_gameObject, std::shared_ptr<Sound> p_other): IComponent{ p_gameObject, SOUND }
{
    m_soundFile = p_other->m_soundFile;
    m_maxDistance = p_other->m_maxDistance;
    m_minDistance = p_other->m_minDistance;
    m_playLooped = p_other->m_playLooped;
    m_playSoundIn3D = p_other->m_playSoundIn3D;
    m_sound = p_other->m_sound;
    m_startPaused = p_other->m_startPaused;
    m_volume = p_other->m_volume;
    m_isActive = p_other->m_isActive;
    m_name = p_other->m_name;

    Systems::SoundSystem::AddSound(std::shared_ptr<Components::Sound>(this));
}

Engine::Components::Sound::~Sound()
{
    Stop();
}

void Engine::Components::Sound::Serialize(std::ostream& p_stream)
{
    p_stream << typeid(*this).name() << "\n{\n" <<
        "   m_playLooped " << m_playLooped << "\n" <<
        "   m_startPaused " << m_startPaused << "\n" <<
        "   m_playSoundIn3D " << m_playSoundIn3D << "\n" <<
        "   m_minDistance " << m_minDistance << "\n" <<
        "   m_maxDistance " << m_maxDistance << "\n" <<
        "   m_volume " << m_volume << "\n" <<
        "   m_soundFile " << m_soundFile << "\n" <<
        "}\n";
}

void Engine::Components::Sound::Deserialize(Objects::GameObject* p_gameObject, std::vector<std::string>& p_block)
{
    m_gameObject = p_gameObject;

    std::vector <std::string> words;

    for (auto& line : p_block)
    {
        std::stringstream stringStream(line);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (words[0] == "m_playLooped")
        {
            m_playLooped = std::stoi(words[1]);
        }
        else if (words[0] == "m_startPaused")
        {
            m_startPaused = std::stoi(words[1]);
        }
        else if (words[0] == "m_playSoundIn3D")
        {
            m_playSoundIn3D = std::stoi(words[1]);
        }
        else if (words[0] == "m_minDistance")
        {
            m_minDistance = std::stof(words[1]);
        }
        else if (words[0] == "m_maxDistance")
        {
            m_maxDistance = std::stof(words[1]);
        }
        else if (words[0] == "m_volume")
        {
            m_volume = std::stof(words[1]);
        }
        else if (words[0] == "m_soundFile")
        {
            m_soundFile = words[1];
        }


        words.clear();
    }
}

bool Engine::Components::Sound::DeleteFromMemory()
{
    return false;
}

bool Engine::Components::Sound::operator==(IComponent* p_other)
{
    return false;
}


bool Engine::Components::Sound::RemoveComponent()
{
    Systems::SoundSystem::RemoveSound(m_id);
    Containers::ComponentContainer::RemoveComponent(m_id);
    return true;
}

void Engine::Components::Sound::PlaySound()
{
    if (m_isPlaying)
        return;

    const auto& position = m_gameObject->GetTransform()->GetPosition();

    if (m_playSoundIn3D)
    {
        m_sound = Systems::SoundSystem::GetSoundEngine()->play3D(m_soundFile.c_str(),
            irrklang::vec3df(position.x, position.y, -position.z), m_playLooped, m_startPaused, true);
    }
    else
        m_sound = Systems::SoundSystem::GetSoundEngine()->play2D(m_soundFile.c_str(), m_playLooped, m_startPaused, true);

    if (m_sound)
    {
        m_isPlaying = true;
        m_sound->setVolume(m_volume);
        m_sound->setMinDistance(m_minDistance);
        m_sound->setMaxDistance(m_maxDistance);
    }
}

void Engine::Components::Sound::Pause()
{
    if(m_sound)
        m_sound->setIsPaused(!m_sound->getIsPaused());
}

void Engine::Components::Sound::Stop()
{
    if (m_sound)
        m_sound->stop();
}

void Engine::Components::Sound::SetActive(bool p_active)
{
    m_isActive = p_active;
}

void Engine::Components::Sound::SetVolume(float p_volume)
{
    m_volume = std::clamp(p_volume, 0.0f, 1.0f);

    if (m_sound)
        m_sound->setVolume(m_volume);
}
