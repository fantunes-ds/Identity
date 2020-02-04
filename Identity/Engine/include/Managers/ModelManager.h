#pragma once
#include <Export.h>
#include <map>
#include <Tools/IDCounter.h>
#include <3DLoader/ObjectElements/Model.h>


namespace Engine::Managers
{
    /**
     * @brief Singleton data holder that stores all loaded Models and Meshes.
     */
    class API_ENGINE ModelManager
    {
    public:
        ~ModelManager();
        ModelManager(const ModelManager&) = delete;
        ModelManager(const ModelManager&&) = delete;

        static ModelManager* GetInstance();

        /**
         *@brief Load a Model and store it in ModelManager.
         *@return Pointer to the newly added Model.
         */

        //TODO: determine if return pointer or id?
        static std::shared_ptr<Engine::ObjectElements::Model> AddModel(const std::string& p_path, const std::string& p_name);

        /**
         * @brief Finds an already loaded Model according to name.
         * @return ID of the Model that has the name mentioned as a parameter.
         */
        static int32_t FindModel(const std::string& p_name);
        static std::shared_ptr<ObjectElements::Model> FindModel(uint32_t p_id);
        inline std::map<int, std::shared_ptr<ObjectElements::Model>>& GetAllModels() { return m_models; }

        inline void SetGraphicsDevice(Microsoft::WRL::ComPtr<ID3D11Device> p_device) { m_graphicsDevice = p_device; }

    private:
        ModelManager() = default;

        inline static ModelManager* m_instance = nullptr;
        Microsoft::WRL::ComPtr<ID3D11Device> m_graphicsDevice;
        std::map<int, std::shared_ptr<ObjectElements::Model>> m_models;
    };
}
