#include <stdafx.h>
#include <Containers/LightContainer.h>

Engine::Containers::LightContainer::~LightContainer()
{
    delete m_instance;
}

Engine::Containers::LightContainer* Engine::Containers::LightContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new LightContainer();
    }

    return m_instance;
}

void Engine::Containers::LightContainer::AddLight(Rendering::ILight* p_light)
{
    /*int32_t id = p_light->GetID();

    GetInstance()->m_lights.insert_or_assign(id, std::make_shared<Rendering::ILight>(*p_light));

    return id;*/
}
