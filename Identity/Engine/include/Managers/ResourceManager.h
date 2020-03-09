#pragma once
#include <Export.h>

#include <map>
#include <string>
#include <memory>
#include <3DLoader/ObjectElements/Model.h>

namespace Engine::Managers
{
    class API_ENGINE ResourceManager
    {
    public:
        ~ResourceManager() = default;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(const ResourceManager&&) = delete;

        static std::unique_ptr<ResourceManager>& GetInstance();
        static std::shared_ptr<ObjectElements::Model> AddModel(const std::string& p_path, const std::string& p_name);
        static std::shared_ptr<ObjectElements::Model> GetModel(const std::string& p_name);
        static std::vector<std::shared_ptr<ObjectElements::Model>> GetAllModels();

        //TODO: do the same with texture and shaders


    private:
        ResourceManager() = default;

        inline static std::unique_ptr<ResourceManager> m_instance{ nullptr };
        std::map<std::string, std::shared_ptr<ObjectElements::Model>> m_models;

    };
}
