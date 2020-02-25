#pragma once
#include <Export.h>

namespace Engine::Rendering
{
    class ILight;
}

namespace Engine::Containers
{
    class API_ENGINE LightContainer
    {
    public:
        ~LightContainer();

        LightContainer(const LightContainer&) = delete;
        LightContainer(const LightContainer&&) = delete;

        static LightContainer* GetInstance();

        static void AddLight(Rendering::ILight* p_light);

    private:
        LightContainer() = default;

        inline static LightContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Rendering::ILight>> m_lights;
    };
}
