#pragma once
#include <Export.h>
#include <Scene/SceneGraph/SceneGraph.h>

namespace Engine::Scene
{
    class API_ENGINE Scene: public Objects::IObject
    {
    public:
        Scene();
        ~Scene();

    private:
        SceneGraph m_sceneGraph;
    };
}
