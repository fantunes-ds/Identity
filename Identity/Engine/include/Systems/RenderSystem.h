#pragma once
#include <Export.h>
#include <Systems/IECSSystem.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Renderer.h>
#include <Rendering/Lights/Light.h>
#include "Events/Event.h"
#include "Rendering/Materials/Texture.h"

namespace Engine::Systems
{
    class API_ENGINE RenderSystem: public IECSSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem() = default;

        void DrawScene(float p_deltaTime);
        void Update(float p_deltaTime) override;

        void ResetActiveCamera();

        void SetActiveCamera(int32_t p_id);

    private:
        std::map<int, std::shared_ptr<Rendering::Lights::Light>> m_lights;

        int32_t m_activeCamera = -1;
    };
}
