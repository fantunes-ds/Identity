#include <stdafx.h>

#include <Containers/LightContainer.h>
#include <Rendering/Lights/DirectionalLight.h>

using namespace Engine::Containers;

LightContainer::~LightContainer()
{
    delete m_instance;
}

LightContainer* LightContainer::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new LightContainer();
    }

    return m_instance;
}

int32_t LightContainer::AddLight(Rendering::Lights::ILight* p_light)
{
    /*for (auto& light : GetInstance()->m_lights)
    {
        if (typeid(*light.second) == typeid(*p_light))
        {
            if (*light.second == p_light)
            {
                std::string type = typeid(*p_light).name();
                const std::string error("ComponentContainer::AddComponent<" + type + ">(Components::IComponent* p_component): Tried to add a Component that already exists");
                MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
                return light.first;
            }
        }
    }*/

    GetInstance()->m_lights.insert_or_assign(p_light->GetID(), std::shared_ptr<Engine::Rendering::Lights::ILight>(p_light));
    return p_light->GetID();
}
