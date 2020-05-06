#pragma once
#include <Export.h>
#include <map>
// #include <memory>
#include <Systems/ISystem.h>
#include <Components/Light.h>

// namespace Engine::Components
// {
//     class Light;
// }

namespace Engine::Systems
{
    class API_ENGINE LightSystem : public ISystem
    {
    public:
        ~LightSystem()                   = default;
        LightSystem(const LightSystem&)  = delete;
        LightSystem(const LightSystem&&) = delete;

        static LightSystem* GetInstance();

        inline static int32_t AddLight(std::shared_ptr<Components::Light> p_light) { return GetInstance()->AddLightNS(p_light); }
        int32_t AddLightNS(std::shared_ptr<Components::Light> p_light);
        static bool    RemoveLight(int32_t p_id);

        static void Update(const float p_deltaTime, bool p_isEditor);
        void        IUpdate(const float p_deltaTime, bool p_isEditor = false) override;

        static std::shared_ptr<Components::Light> GetLight(int32_t p_id);
        static std::vector<std::shared_ptr<Components::Light>> GetLights();
        static std::map<int32_t, std::shared_ptr<Components::Light>>& GetAllLights() { return GetInstance()->m_lights; }

    private:
        LightSystem() = default;

        static inline LightSystem*                            m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<Components::Light>> m_lights;
    };
}
