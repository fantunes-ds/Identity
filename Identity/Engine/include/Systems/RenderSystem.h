#pragma once
#include <Export.h>
#include <Systems/IECSSystem.h>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Renderer.h>
#include <Rendering/Lights/Light.h>
#include "Events/Event.h"
#include "Rendering/Materials/Texture.h"
#include "Scene/SceneGraph/SceneGraph.h"

namespace Engine::Systems
{
    class API_ENGINE RenderSystem: public IECSSystem
    {
    public:
        RenderSystem();
        virtual ~RenderSystem() = default;

        void DrawScene();
        void DrawSceneNode(std::shared_ptr<Scene::SceneNode> p_sceneNode);

        void Update() override;

        /**
         * @brief As soon as we have a LightContainer, this method should be deleted.
         * @return returns the new light's id if successful, -1 if unsuccessful.
         */
        uint32_t AddLight(Rendering::Lights::Light& p_light);

        void ResetActiveCamera();

        void SetActiveCamera(int32_t p_id);

    private:
        std::map<int32_t, std::shared_ptr<Rendering::Lights::Light>> m_lights;
        int32_t m_activeCamera = -1;
    };
}
