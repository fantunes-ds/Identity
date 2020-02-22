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
        RenderSystem();
        virtual ~RenderSystem() = default;

        void DrawScene();
        void Update() override;

        /**
         * @brief As soon as we have a LightContainer, this method should be deleted.
         * @return returns the new light's id if successful, -1 if unsuccessful.
         */
        uint32_t AddLight(Rendering::Light& p_light);

        void ResetActiveCamera();

        void SetActiveCamera(int32_t p_id);

    private:
        std::map<int, std::shared_ptr<Rendering::Light>> m_lights;

        int32_t m_activeCamera = -1;
    };
}
