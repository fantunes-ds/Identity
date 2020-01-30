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
        std::shared_ptr<Engine::ObjectElements::Model> AddModel(const std::string& p_path, const std::string& p_name);
        std::shared_ptr<ObjectElements::Model> FindModel(const std::string& p_name);
        inline std::vector<std::shared_ptr<ObjectElements::Model>>& GetAllModels() { return m_models; }

        inline void SetGraphicsDevice(Microsoft::WRL::ComPtr<ID3D11Device> p_device) { m_graphicsDevice = p_device; }

    private:
        ModelManager() = default;

        inline static ModelManager* m_instance = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Device> m_graphicsDevice;
        std::vector<std::shared_ptr<ObjectElements::Model>> m_models;
    };
}
