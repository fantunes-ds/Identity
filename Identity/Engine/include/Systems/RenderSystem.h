#pragma once
#include <Export.h>
#include <Systems/IECSSystem.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Renderer.h>
#include <Rendering/Light.h>

namespace Engine::Systems
{
    class API_ENGINE RenderSystem: public IECSSystem
    {
    public:
        RenderSystem(Rendering::Renderer* p_renderer);
        virtual ~RenderSystem() = default;

        void DrawScene();
        void Update() override;

        /**
         * @brief
         * @return returns the new model's id if successful, -1 if unsuccessful
         */
        uint32_t AddModel(const std::string& p_path, const std::string& p_name);

    private:
        std::map<int, std::shared_ptr<ObjectElements::Model>> m_models;
        std::map<int, std::shared_ptr<Rendering::Light>> m_lights;

        Rendering::Renderer* m_renderer;
    };
}
