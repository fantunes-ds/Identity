#include <stdafx.h>
#include <Managers/ResourceManager.h>
#include <3DLoader/ObjectLoader.h>
#include <Rendering/Renderer.h>
#include <Scene/SceneGraph/SceneGraph.h>

std::unique_ptr<Engine::Managers::ResourceManager>& Engine::Managers::ResourceManager::GetInstance()
{
    if (m_instance == nullptr)
        m_instance = std::make_unique<ResourceManager>();

    return m_instance;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Managers::ResourceManager::AddModel(const std::string& p_path,
    const std::string& p_name)
{
    for (auto model : GetInstance()->m_models)
    {
        if (model->GetPath() == p_path)
        {
            const std::string info("The model located at " + p_path + " is already loaded and will be returned");
            MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
            return model;
        }

        if (model->GetName() == p_name)
        {
            const std::string info("The name '" + p_name + "' is already in use, please change the name");
            MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
            return nullptr;
        }
    }

    std::shared_ptr<ObjectElements::Model> model = ObjectLoader::LoadModel(p_path);

    if (model == nullptr)
    {
        const std::string error("ResourceManager::AddModel(const std::string& p_path, const std::string& p_name): Could not load model at " + p_path + " because there was no object to be found at that path");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    model->SetName(p_name);
    model->SetPath(p_path);

    for (auto& mesh : model->GetMeshes())
        mesh->GenerateBuffers(Rendering::Renderer::GetInstance()->GetDevice());

    GetInstance()->m_models.push_back(model);
    Scene::SceneGraph::GetInstance()->AddRootSceneNode(model->GetRootNode());

    return model;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Managers::ResourceManager::GetModel(const std::string& p_name)
{
    for (auto model : GetInstance()->m_models)
    {
        if (model->GetName() == p_name)
            return model;
    }

    return nullptr;
}

std::vector<std::shared_ptr<Engine::ObjectElements::Model>> Engine::Managers::ResourceManager::GetAllModels()
{
    return GetInstance()->m_models;
}

std::shared_ptr<Engine::Rendering::Materials::Texture> Engine::Managers::ResourceManager::AddTexture(
    const std::string& p_path, const std::string& p_name)
{

    for (auto texture : GetInstance()->m_textures)
    {
        if (texture->GetPath() == p_path)
        {
            const std::string info("The texture located at " + p_path + " is already loaded and will be returned");
            MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
            return texture;
        }

        if (texture->GetName() == p_name)
        {
            const std::string info("The name '" + p_name + "' is already in use, please change the name");
            MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
            return nullptr;
        }
    }

    std::shared_ptr<Rendering::Materials::Texture> texture = Rendering::Materials::Texture::LoadTexture(p_path, p_name);

    if (texture == nullptr)
    {
        const std::string error("ResourceManager::AddTexture(const std::string& p_path, const std::string& p_name): Could not load texture at " + p_path + " because there was no object to be found at that path");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    texture->SetName(p_name);
    texture->SetPath(p_path);

    GetInstance()->m_textures.push_back(texture);

    return texture;
}

std::shared_ptr<Engine::Rendering::Materials::Texture> Engine::Managers::ResourceManager::GetTexture(
    const std::string& p_name)
{
    for (auto texture : GetInstance()->m_textures)
    {
        if (texture->GetName() == p_name)
            return texture;
    }

    return nullptr;
}

std::vector<std::shared_ptr<Engine::Rendering::Materials::Texture>> Engine::Managers::ResourceManager::GetAllTextures()
{
    return GetInstance()->m_textures;
}

std::shared_ptr<Engine::Rendering::Materials::Shader> Engine::Managers::ResourceManager::AddShader(
    const std::string& p_path, const std::string& p_name)
{
    return nullptr;
}

std::shared_ptr<Engine::Rendering::Materials::Shader> Engine::Managers::ResourceManager::GetShader(
    const std::string& p_name)
{
    return nullptr;
}

std::vector<std::shared_ptr<Engine::Rendering::Materials::Shader>> Engine::Managers::ResourceManager::GetAllShaders()
{
    return GetInstance()->m_shaders;
}
