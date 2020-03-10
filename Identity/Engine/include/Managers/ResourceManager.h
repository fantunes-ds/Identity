#pragma once
#include <Export.h>

#include <map>
#include <string>
#include <memory>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Materials/Texture.h>
#include <Rendering/Materials/Shader.h>

namespace Engine::Managers
{
    class API_ENGINE ResourceManager
    {
    public:
        ~ResourceManager() = default;
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(const ResourceManager&&) = delete;

        /**
         * @brief Used to get the instance of the resource manager
         * @return Return a reference to the unique_ptr of the instance
         */
        static std::unique_ptr<ResourceManager>& GetInstance();

#pragma region Model
        /**
         * @brief Used to add a model to the resource manager
         * @param p_path The path of the model you want to load
         * @param p_name The name you want to give the model
         * @return Return a shared_ptr of the loaded model
         */
        static std::shared_ptr<ObjectElements::Model> AddModel(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a model from the resource manager
         * @param p_name The name of the model you want to get
         * @return Return a shared_ptr of the model or nullptr if it was not found
         */
        static std::shared_ptr<ObjectElements::Model> GetModel(const std::string& p_name);
        /**
         * @brief Used to get all model from the resource manager
         * @return Return a vector of shared_ptr of all the model
         */
        static std::vector<std::shared_ptr<ObjectElements::Model>> GetAllModels();
#pragma endregion

#pragma region Texture
        /**
         * @brief Used to add a texture to the resource manager
         * @param p_path The path of the texture you want to load
         * @param p_name The name you want to give the texture
         * @return Return a shared_ptr of the loaded texture
         */
        static std::shared_ptr<Rendering::Materials::Texture> AddTexture(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a texture from the resource manager
         * @param p_name The name of the texture you want to get
         * @return Return a shared_ptr of the texture or nullptr if it was not found
         */
        static std::shared_ptr<Rendering::Materials::Texture> GetTexture(const std::string& p_name);
        /**
         * @brief Used to get all texture from the resource manager
         * @return Return a vector of shared_ptr of all the texture
         */
        static std::vector<std::shared_ptr<Rendering::Materials::Texture>> GetAllTextures();
#pragma endregion

#pragma region Shader
        /**
         * @brief Used to add a shader to the resource manager
         * @param p_path The path of the shader you want to load
         * @param p_name The name you want to give the shader
         * @return Return a shared_ptr of the loaded shader
         */
        static std::shared_ptr<Rendering::Materials::Shader> AddShader(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a shader from the resource manager
         * @param p_name The name of the shader you want to get
         * @return Return a shared_ptr of the shader or nullptr if it was not found
         */
        static std::shared_ptr<Rendering::Materials::Shader> GetShader(const std::string& p_name);
        /**
         * @brief Used to get all shader from the resource manager
         * @return Return a vector of shared_ptr of all the shader
         */
        static std::vector<std::shared_ptr<Rendering::Materials::Shader>> GetAllShaders();
#pragma endregion


        ResourceManager() = default;
    private:

        inline static std::unique_ptr<ResourceManager> m_instance{ nullptr };
        std::vector<std::shared_ptr<ObjectElements::Model>> m_models;
        std::vector<std::shared_ptr<Rendering::Materials::Texture>> m_textures;
        std::vector<std::shared_ptr<Rendering::Materials::Shader>> m_shaders;
    };
}
