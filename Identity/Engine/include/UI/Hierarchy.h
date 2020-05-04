#pragma once

#include <Scene/SceneGraph/SceneNode.h>

#include <Export.h>

namespace Engine::UI
{
    class Hierarchy
    {
    public :
        Hierarchy() = delete;
        ~Hierarchy() = delete;

        API_ENGINE static void CreateHierarchy();
        static std::shared_ptr<Scene::SceneNode> DisplayNextChild(std::shared_ptr<Scene::SceneNode> p_child, int& p_i);
    };
}
