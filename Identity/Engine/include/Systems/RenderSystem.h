#pragma once
#include <Export.h>
#include <Systems/ISystem.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Renderer.h>
#include <Rendering/Lights/Light.h>
#include "Events/Event.h"
#include "Rendering/Materials/Texture.h"
#include "Scene/SceneGraph/SceneGraph.h"

namespace Engine::Systems
{
    class API_ENGINE RenderSystem: public ISystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem() = default;

        void DrawScene(float p_deltaTime);
        void DrawSceneNode(std::shared_ptr<Scene::SceneNode> p_sceneNode);
        void IUpdate(float p_deltaTime) override;

        void ResetActiveCamera();

        void SetActiveCamera(int32_t p_id);

    private:
        std::map<int32_t, std::shared_ptr<Rendering::Lights::Light>> m_lights;
        int32_t m_activeCamera = -1;
    };
}
