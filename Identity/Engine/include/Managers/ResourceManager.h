#pragma once
#include <Export.h>

#include <string>
#include <memory>
#include <3DLoader/ObjectElements/Model.h>
#include <Rendering/Materials/Texture.h>
#include <Rendering/Materials/PixelShader.h>
#include <Rendering/Materials/VertexShader.h>
#include <Rendering/Materials/Material.h>

namespace Engine::Managers
{
    class API_ENGINE ResourceManager
    {
    public:
        // ResourceManager();
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
         * @param p_name The name you want to give to the model
         * @return Return a shared_ptr of the loaded model
         */
        static const int32_t AddModel(const std::string& p_path, const std::string& p_name);

        //TODO add comments here
        static const int32_t AddModel(const ObjectElements::Model& p_model);

        /**
         * @brief Used to get a model from the resource manager
         * @param p_name The name of the model you want to get
         * @return Return a shared_ptr of the model or nullptr if it was not found
         */
        static const int32_t GetModel(const std::string& p_name);

        //TODO add comments here
        static std::shared_ptr<ObjectElements::Model> FindModel(const int32_t p_id);

        /**
         * @brief Used to get all model from the resource manager
         * @return Return a vector of shared_ptr of all the model
         */
        static std::vector<std::shared_ptr<ObjectElements::Model>> GetAllModels();

        //TODO add model here
        static bool RemoveModel(const int32_t p_id);

#pragma endregion

#pragma region Texture
        /**
         * @brief Used to add a texture to the resource manager
         * @param p_path The path of the texture you want to load
         * @param p_name The name you want to give to the texture
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
#pragma region Pixel
        /**
         * @brief Used to add a pixel shader to the resource manager
         * @param p_path The path of the pixel shader you want to load
         * @param p_name The name you want to give to the pixel shader
         * @return Return a shared_ptr of the loaded pixel shader
         */
        static std::shared_ptr<Rendering::Materials::PixelShader> AddPixelShader(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a pixel shader from the resource manager
         * @param p_name The name of the pixel shader you want to get
         * @return Return a shared_ptr of the pixel shader or nullptr if it was not found
         */
        static std::shared_ptr<Rendering::Materials::PixelShader> GetPixelShader(const std::string& p_name);
        /**
         * @brief Used to get all pixel shader from the resource manager
         * @return Return a vector of shared_ptr of all the pixel shader
         */
        static std::vector<std::shared_ptr<Rendering::Materials::PixelShader>> GetAllPixelShaders();
#pragma endregion

#pragma region Vertex
        /**
         * @brief Used to add a vertex shader to the resource manager
         * @param p_path The path of the vertex shader you want to load
         * @param p_name The name you want to give to the vertex shader
         * @return Return a shared_ptr of the loaded vertex shader
         */
        static std::shared_ptr<Rendering::Materials::VertexShader> AddVertexShader(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a vertex shader from the resource manager
         * @param p_name The name of the vertex shader you want to get
         * @return Return a shared_ptr of the vertex shader or nullptr if it was not found
         */
        static std::shared_ptr<Rendering::Materials::VertexShader> GetVertexShader(const std::string& p_name);
        /**
         * @brief Used to get all vertex shader from the resource manager
         * @return Return a vector of shared_ptr of all the vertex shader
         */
        static std::vector<std::shared_ptr<Rendering::Materials::VertexShader>> GetAllVertexShaders();
#pragma endregion
#pragma endregion

#pragma region Material
        //TODO add a 'AddMaterial' method that will load a material from a file

        /**
         * @brief Used to create a new material from a pixel shader, a vertex shader and a texture
         * @param p_name The name of the new material
         * @param p_pixelShaderName The name of the pixel shader you want to use in the new material
         * @param p_vertexShaderName The name of the vertex shader you want to use in the new material
         * @param p_textureName [optional] The name of the texture you want to use in the new material
         * @return Return a share_ptr of the created material
         */
        static std::shared_ptr<Rendering::Materials::Material> CreateMaterial(const std::string& p_name, const std::string& p_pixelShaderName,
                                                                              const std::string& p_vertexShaderName, const std::string& p_textureName = "");
        /**
         * @brief Used to get a material from the resource manager
         * @param p_name The name of the material you want to get
         * @return Return a shared_ptr of the material or nullptr if it was not found
         */
        static std::shared_ptr<Rendering::Materials::Material> GetMaterial(const std::string& p_name);

        /**
         * @brief Used to get all material from the resource manager
         * @return Return a vector of shared_ptr of all the material
         */
        static std::vector<std::shared_ptr<Rendering::Materials::Material>> GetAllMaterials();
#pragma endregion

        ResourceManager();
    private:

#pragma region Model
        /**
         * @brief Used to add a model to the resource manager
         * @param p_path The path of the model you want to load
         * @param p_name The name you want to give to the model
         * @return Return a int32_t of the loaded model
         */
        const int32_t AddModelNS(const std::string& p_path, const std::string& p_name);

        //TODO add comments here
        const int32_t AddModelNS(const ObjectElements::Model& p_model);

        /**
         * @brief Used to get a model from the resource manager
         * @param p_name The name of the model you want to get
         * @return Return a int32_t of the model or -1 if it was not found
         */
        const int32_t GetModelNS(const std::string& p_name);

        //TODO add comments here
        std::shared_ptr<ObjectElements::Model> FindModelNS(const int32_t p_id);

        /**
         * @brief Used to get all model from the resource manager
         * @return Return a vector of shared_ptr of all the model
         */
        std::vector<std::shared_ptr<ObjectElements::Model>> GetAllModelsNS();

        //TODO add comments here
        bool RemoveModelNS(const int32_t p_id);

#pragma endregion

#pragma region Texture
        /**
         * @brief Used to add a texture to the resource manager
         * @param p_path The path of the texture you want to load
         * @param p_name The name you want to give to the texture
         * @return Return a shared_ptr of the loaded texture
         */
        std::shared_ptr<Rendering::Materials::Texture> AddTextureNS(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a texture from the resource manager
         * @param p_name The name of the texture you want to get
         * @return Return a shared_ptr of the texture or nullptr if it was not found
         */
        std::shared_ptr<Rendering::Materials::Texture> GetTextureNS(const std::string& p_name);
        /**
         * @brief Used to get all texture from the resource manager
         * @return Return a vector of shared_ptr of all the texture
         */
        std::vector<std::shared_ptr<Rendering::Materials::Texture>> GetAllTexturesNS();
#pragma endregion

#pragma region Shader
#pragma region Pixel
        /**
         * @brief Used to add a pixel shader to the resource manager
         * @param p_path The path of the pixel shader you want to load
         * @param p_name The name you want to give to the pixel shader
         * @return Return a shared_ptr of the loaded pixel shader
         */
        std::shared_ptr<Rendering::Materials::PixelShader> AddPixelShaderNS(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a pixel shader from the resource manager
         * @param p_name The name of the pixel shader you want to get
         * @return Return a shared_ptr of the pixel shader or nullptr if it was not found
         */
        std::shared_ptr<Rendering::Materials::PixelShader> GetPixelShaderNS(const std::string& p_name);
        /**
         * @brief Used to get all pixel shader from the resource manager
         * @return Return a vector of shared_ptr of all the pixel shader
         */
        std::vector<std::shared_ptr<Rendering::Materials::PixelShader>> GetAllPixelShadersNS();
#pragma endregion

#pragma region Vertex
        /**
         * @brief Used to add a vertex shader to the resource manager
         * @param p_path The path of the vertex shader you want to load
         * @param p_name The name you want to give to the vertex shader
         * @return Return a shared_ptr of the loaded vertex shader
         */
        std::shared_ptr<Rendering::Materials::VertexShader> AddVertexShaderNS(const std::string& p_path, const std::string& p_name);
        /**
         * @brief Used to get a vertex shader from the resource manager
         * @param p_name The name of the vertex shader you want to get
         * @return Return a shared_ptr of the vertex shader or nullptr if it was not found
         */
        std::shared_ptr<Rendering::Materials::VertexShader> GetVertexShaderNS(const std::string& p_name);
        /**
         * @brief Used to get all vertex shader from the resource manager
         * @return Return a vector of shared_ptr of all the vertex shader
         */
        std::vector<std::shared_ptr<Rendering::Materials::VertexShader>> GetAllVertexShadersNS();
#pragma endregion
#pragma endregion

#pragma region Material
        //TODO add a 'AddMaterial' method that will load a material from a file

        /**
         * @brief Used to create a new material from a pixel shader, a vertex shader and a texture
         * @param p_name The name of the new material
         * @param p_pixelShaderName The name of the pixel shader you want to use in the new material
         * @param p_vertexShaderName The name of the vertex shader you want to use in the new material
         * @param p_textureName [optional] The name of the texture you want to use in the new material
         * @return Return a share_ptr of the created material
         */
        std::shared_ptr<Rendering::Materials::Material> CreateMaterialNS(const std::string& p_name, const std::string& p_pixelShaderName,
            const std::string& p_vertexShaderName, const std::string& p_textureName = "");
        /**
         * @brief Used to get a material from the resource manager
         * @param p_name The name of the material you want to get
         * @return Return a shared_ptr of the material or nullptr if it was not found
         */
        std::shared_ptr<Rendering::Materials::Material> GetMaterialNS(const std::string& p_name);

        /**
         * @brief Used to get all material from the resource manager
         * @return Return a vector of shared_ptr of all the material
         */
        std::vector<std::shared_ptr<Rendering::Materials::Material>> GetAllMaterialsNS();
#pragma endregion

        inline static std::unique_ptr<ResourceManager> m_instance{ nullptr };

        std::map<int32_t, std::shared_ptr<ObjectElements::Model>> m_models;
        std::vector<std::shared_ptr<Rendering::Materials::Texture>> m_textures;
        std::vector<std::shared_ptr<Rendering::Materials::PixelShader>> m_pixelShaders;
        std::vector<std::shared_ptr<Rendering::Materials::VertexShader>> m_vertexShaders;
        std::vector<std::shared_ptr<Rendering::Materials::Material>> m_materials;
    };
}
