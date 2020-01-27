#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Model.h>


namespace Engine::Manager
{
    class API_ENGINE ModelManager
    {
    public:
        ~ModelManager();
        ModelManager(const ModelManager&) = delete;
        ModelManager(const ModelManager&&) = delete;

        static ModelManager* GetInstance();
        void AddModel(const std::string& p_path, const std::string& p_name);
        std::shared_ptr<ObjectElements::Model> FindModel(const std::string& p_name);

    private:
        ModelManager() = default;

        inline static ModelManager* m_instance = nullptr;
        std::vector<std::shared_ptr<ObjectElements::Model>> m_models;
    };
}
