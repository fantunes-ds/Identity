#pragma once

#include <Scene/SceneGraph/SceneNode.h>

#include <Export.h>

#include "Core/App.h"

namespace Engine::UI
{
    class Hierarchy
    {
    public :
        Hierarchy() = delete;
        ~Hierarchy() = delete;

        API_ENGINE static void CreateHierarchy(Core::App& p_appRef);
        static void CallInspector(int32_t p_id);
        static std::shared_ptr<Scene::SceneNode> DisplayNextChild(std::shared_ptr<Scene::SceneNode> p_child);

        static int m_currentlySelected;
    };
}
