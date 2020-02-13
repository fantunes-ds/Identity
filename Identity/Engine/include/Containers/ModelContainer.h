#pragma once
#include <Export.h>
#include <map>
#include <Tools/IDCounter.h>
#include <3DLoader/ObjectElements/Model.h>


namespace Engine::Containers
{
    /**
     * @brief Singleton data holder that stores all loaded Models and Meshes.
     */
    class API_ENGINE ModelContainer
    {
    public:
        ~ModelContainer();
        ModelContainer(const ModelContainer&) = delete;
        ModelContainer(const ModelContainer&&) = delete;

        static ModelContainer* GetInstance();

        /**
         *@brief Load a Model and store it in ModelContainer.
         *@return Pointer to the newly added Model.
         */

        //TODO: determine if return pointer or id?
        static std::shared_ptr<Engine::ObjectElements::Model> AddModel(const std::string& p_path, const std::string& p_name);
        static bool RemoveModel(int32_t p_id);
        /**
         * @brief Finds an already loaded Model according to name.
         * @return ID of the Model that has the name mentioned as a parameter.
         */
        static int32_t FindModel(const std::string& p_name);

        /**
         * @brief Finds an already loaded Model according to ID.
         * @return pointer of the Model that has the name mentioned as a parameter.
         */
        static std::shared_ptr<ObjectElements::Model> FindModel(uint32_t p_id);
        static inline std::map<int, std::shared_ptr<ObjectElements::Model>>& GetAllModels() { return GetInstance()->m_models; }

    private:
        ModelContainer() = default;

        inline static ModelContainer* m_instance = nullptr;
        std::map<int32_t, std::shared_ptr<ObjectElements::Model>> m_models;
    };
}
