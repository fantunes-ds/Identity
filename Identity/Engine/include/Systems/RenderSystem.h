#pragma once
#include <Export.h>
#include <Systems/ISystem.h>
#include <Scene/SceneGraph/SceneGraph.h>

namespace Engine::Components
{
    class Camera;
}

namespace Engine::Systems
{
    class API_ENGINE RenderSystem: public ISystem
    {
    public:
        ~RenderSystem();

        static void DrawScene(float p_deltaTime, bool p_isEditor);
        static void DrawSceneNode(std::shared_ptr<Scene::SceneNode> p_sceneNode);
        void IUpdate(float p_deltaTime, bool p_isEditor = false) override;
        static void ResetActiveCamera();

        static void SetActiveCamera(int32_t p_id);

        static RenderSystem* GetInstance();
        static std::shared_ptr<Components::Camera> GetActiveCamera();

    private:
        RenderSystem() = default;

        inline static RenderSystem* m_instance;
        int32_t m_activeCamera = -1;
    };
}
