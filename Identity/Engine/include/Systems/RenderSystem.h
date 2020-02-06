#pragma once
#include <Export.h>
#include <Systems/IECSSystem.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Renderer.h>
#include <Rendering/Light.h>
#include "Events/Event.h"

namespace Engine::Systems
{
    class API_ENGINE RenderSystem: public IECSSystem
    {
    public:
        RenderSystem(Rendering::Renderer* p_renderer);
        virtual ~RenderSystem() = default;

        void DrawScene();
        void Update() override;
        void UpdateCamera();

        /**
         * @brief Adds a model to the model container.
         * @return returns the new model's id if successful, -1 if unsuccessful.
         */
        uint32_t AddModel(const std::string& p_path, const std::string& p_name);

        /**
         * @brief Adds a light to the light container.
         * @return returns the new light's id if successful, -1 if unsuccessful.
         */
        uint32_t AddLight(Rendering::Light& p_light);

        void SetActiveCamera();

    private:
        //std::map<int, std::shared_ptr<ObjectElements::Model>> m_models;
        std::map<int, std::shared_ptr<Rendering::Light>> m_lights;

        //Rendering::Camera m_camera;

        Rendering::Renderer* m_renderer;
        std::shared_ptr<Rendering::Camera> m_activeCamera;
    };
}
