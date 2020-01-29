#pragma once
#include <Export.h>
#include <map>
#include <3DLoader/ObjectElements/Model.h>

namespace Engine::Manager
{
    struct API_ENGINE ModelsData
    {

        std::map<int, std::shared_ptr<ObjectElements::Model>> m_models;
    };
}
