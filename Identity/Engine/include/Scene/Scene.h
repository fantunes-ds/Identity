#pragma once
#include <Export.h>
#include <list>
#include <Scene/SceneGraph/SceneGraph.h>
#include <Objects/GameObject.h>

// namespace Engine::Objects
// {
//     class GameObject;
// }

namespace Engine::Components
{
    class IComponent;
}

namespace Engine::Scene
{
    class API_ENGINE Scene : public Objects::IObject
    {
    public:
        Scene() = default;
        Scene(const std::string& p_name);
        ~Scene();

        void AddGameObject(std::shared_ptr<Objects::GameObject> p_gameObject);
        void RemoveGameObject(std::shared_ptr<Objects::GameObject> p_gameObject);
        void RemoveGameObject(int32_t p_id);

        SceneGraph& GetSceneGraph() { return m_sceneGraph; }
        std::list<std::shared_ptr<Objects::GameObject>> GetAllGameObjectsInScene();
        void SetActiveOnAll(bool p_active);
        void Serialize(std::ostream& p_stream);

    private:
        SceneGraph m_sceneGraph;
    };
}
