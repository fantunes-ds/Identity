#include <stdafx.h>

#include <Windows.h>
#include <Systems/LightSystem.h>
// #include <Components/Light.h>

// using namespace Engine::Systems;

Engine::Systems::LightSystem* Engine::Systems::LightSystem::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new LightSystem();
    }

    return m_instance;
}

int32_t Engine::Systems::LightSystem::AddLightNS(std::shared_ptr<Components::Light> p_light)
{
    int32_t id = p_light->GetID();
    m_lights.insert_or_assign(id, std::shared_ptr<Components::Light>(p_light));
    return id;
}

bool Engine::Systems::LightSystem::RemoveLight(int32_t p_id)
{
    size_t sizeBefore = GetInstance()->m_lights.size();
    GetInstance()->m_lights.erase(p_id);
    size_t sizeAfter = GetInstance()->m_lights.size();

    if (sizeBefore == sizeAfter)
        return false;

    return true;
}

void Engine::Systems::LightSystem::Update(const float p_deltaTime, bool p_isEditor)
{
    GetInstance()->IUpdate(p_deltaTime);
}

void Engine::Systems::LightSystem::IUpdate(const float p_deltaTime, bool p_isEditor)
{
    //TODO complete this
    for (auto light : GetLights())
    {
        if (light->IsActive())
        {
            light->GetLight()->GetLightData().position = {
                light->GetGameObject()->GetTransform()->GetPosition().x,
                light->GetGameObject()->GetTransform()->GetPosition().y,
             light->GetGameObject()->GetTransform()->GetPosition().z, 1.0f };
        }
    }
}

std::shared_ptr<Engine::Components::Light> Engine::Systems::LightSystem::GetLight(int32_t p_id)
{
    if (GetInstance()->m_lights.find(p_id) != GetInstance()->m_lights.end())
        return GetInstance()->m_lights.at(p_id);

    const std::string error("LightSystem::GetLight(int32_t p_id): Didn't find Light with ID " + std::to_string(p_id) + "\n");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<Engine::Components::Light>> Engine::Systems::LightSystem::GetLights()
{
    std::vector<std::shared_ptr<Engine::Components::Light>> lights;

    for (const auto& light : GetAllLights())
    {
        lights.push_back(light.second);
    }

    return lights;
}
