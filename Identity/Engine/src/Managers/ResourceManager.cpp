#include <stdafx.h>

#include <fstream>
#include <3DLoader/ObjectLoader.h>
#include <Managers/ResourceManager.h>
#include <Rendering/Renderer.h>
#include <Scene/SceneGraph/SceneGraph.h>

using namespace Engine::Rendering::Materials;

std::unique_ptr<Engine::Managers::ResourceManager>& Engine::Managers::ResourceManager::GetInstance()
{
    if (m_instance == nullptr)
        m_instance = std::make_unique<ResourceManager>();

    return m_instance;
}

const int32_t Engine::Managers::ResourceManager::AddModel(const std::string& p_path, const std::string& p_name)
{
    return GetInstance()->AddModelNS(p_path, p_name);
}

const int32_t Engine::Managers::ResourceManager::AddModel(const ObjectElements::Model& p_model)
{
    return GetInstance()->AddModelNS(p_model);
}

const int32_t Engine::Managers::ResourceManager::GetModel(const std::string& p_name)
{
    return GetInstance()->GetModelNS(p_name);
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Managers::ResourceManager::FindModel(const int32_t p_id)
{
    return GetInstance()->FindModelNS(p_id);
}

std::vector<std::shared_ptr<Engine::ObjectElements::Model>> Engine::Managers::ResourceManager::GetAllModels()
{
    return GetInstance()->GetAllModelsNS();
}

bool Engine::Managers::ResourceManager::RemoveModel(const int32_t p_id)
{
    return GetInstance()->RemoveModelNS(p_id);
}

std::shared_ptr<Texture> Engine::Managers::ResourceManager::AddTexture(const std::string& p_path, const std::string& p_name)
{
    return GetInstance()->AddTextureNS(p_path, p_name);
}

std::shared_ptr<Texture> Engine::Managers::ResourceManager::GetTexture(const std::string& p_name)
{
    return GetInstance()->GetTextureNS(p_name);
}

std::vector<std::shared_ptr<Texture>> Engine::Managers::ResourceManager::GetAllTextures()
{
    return GetInstance()->GetAllTexturesNS();
}

std::shared_ptr<PixelShader> Engine::Managers::ResourceManager::AddPixelShader(const std::string& p_path, const std::string& p_name)
{
    return GetInstance()->AddPixelShaderNS(p_path, p_name);
}

std::shared_ptr<PixelShader> Engine::Managers::ResourceManager::GetPixelShader(const std::string& p_name)
{
    return GetInstance()->GetPixelShaderNS(p_name);
}

std::vector<std::shared_ptr<PixelShader>> Engine::Managers::ResourceManager::GetAllPixelShaders()
{
    return GetInstance()->GetAllPixelShadersNS();
}

std::shared_ptr<VertexShader> Engine::Managers::ResourceManager::AddVertexShader(const std::string& p_path, const std::string& p_name)
{
    return GetInstance()->AddVertexShaderNS(p_path, p_name);
}

std::shared_ptr<VertexShader> Engine::Managers::ResourceManager::GetVertexShader(const std::string& p_name)
{
    return GetInstance()->GetVertexShaderNS(p_name);
}

std::vector<std::shared_ptr<VertexShader>> Engine::Managers::ResourceManager::GetAllVertexShaders()
{
    return GetInstance()->GetAllVertexShadersNS();
}

std::shared_ptr<Material> Engine::Managers::ResourceManager::CreateMaterial(const std::string& p_name, const std::string& p_pixelShaderName,
                                                          const std::string& p_vertexShaderName, const std::string& p_textureName)
{
    return GetInstance()->CreateMaterialNS(p_name, p_pixelShaderName, p_vertexShaderName, p_textureName);
}

std::shared_ptr<Material> Engine::Managers::ResourceManager::GetMaterial(const std::string& p_name)
{
    return GetInstance()->GetMaterialNS(p_name);
}

std::vector<std::shared_ptr<Material>> Engine::Managers::ResourceManager::GetAllMaterials()
{
    return GetInstance()->GetAllMaterialsNS();
}

Engine::Managers::ResourceManager::ResourceManager()
{
    AddPixelShaderNS("../Engine/Resources/Shaders/PixelShader.cso", "defaultPS");
    AddVertexShaderNS("../Engine/Resources/Shaders/VertexShader.cso", "defaultVS");
    CreateMaterialNS("default", "defaultPS", "defaultVS");
    CreateMaterialNS("RenderText", "defaultPS", "defaultVS");
}

const int32_t Engine::Managers::ResourceManager::AddModelNS(const std::string& p_path, const std::string& p_name)
{
    if (p_name.empty())
    {
        const std::string info("No name was entered for the creation of the model. Please create it with a name");
        MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
        return -1;
    }

    for (auto model : m_models)
    {
        if (model.second->GetPath() == p_path)
        {
            const std::string info("The model located at " + p_path + " is already loaded and will be returned");
            MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
            return model.first;
        }

        if (model.second->GetName() == p_name)
        {
            const std::string info("The name '" + p_name + "' is already in use, please change the name");
            MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
            return -1;
        }
    }

    std::shared_ptr<ObjectElements::Model> model = ObjectLoader::LoadModel(p_path);

    if (model == nullptr)
    {
        const std::string error{ "ResourceManager::AddModel(const std::string& p_path, const std::string& p_name): Could not load model at "
                                + p_path + " because there was no object to be found at that path" };
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return -1;
    }

    model->SetName(p_name);
    model->SetPath(p_path);

    for (auto& mesh : model->GetMeshes())
        mesh->GenerateBuffers(Rendering::Renderer::GetInstance()->GetDevice());

    m_models.insert_or_assign(model->GetID(), model);

    return model->GetID();
}

const int32_t Engine::Managers::ResourceManager::AddModelNS(const ObjectElements::Model& p_model)
{
    auto model = std::make_shared<ObjectElements::Model>(p_model);
    GetInstance()->m_models.insert_or_assign(p_model.GetID(), model);
    return model->GetID();
}

const int32_t Engine::Managers::ResourceManager::GetModelNS(const std::string& p_name)
{
    for (auto model : m_models)
    {
        if (model.second->GetName() == p_name)
            return model.first;
    }

    const std::string error("The model: " + p_name + " does not exist");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONERROR | MB_OK);
    return -1;
}

std::shared_ptr<Engine::ObjectElements::Model> Engine::Managers::ResourceManager::FindModelNS(const int32_t p_id)
{
    for (auto model : m_models)
    {
        if (model.first == p_id)
            return model.second;
    }

    //const std::string error("The model with id: " + std::to_string(p_id) + " does not exist");
    //MessageBox(nullptr, error.c_str(), "Error", MB_ICONERROR | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<Engine::ObjectElements::Model>> Engine::Managers::ResourceManager::GetAllModelsNS()
{
    std::vector<std::shared_ptr<ObjectElements::Model>> tmpVec;

    for (auto model : m_models)
    {
        tmpVec.push_back(model.second);
    }

    return tmpVec;
}

bool Engine::Managers::ResourceManager::RemoveModelNS(const int32_t p_id)
{
    const size_t sizeBefore = GetInstance()->m_models.size();
    GetInstance()->m_models.erase(p_id);
    const size_t sizeAfter = GetInstance()->m_models.size();

    if (sizeBefore == sizeAfter)
        return false;

    return true;
}

std::shared_ptr<Texture> Engine::Managers::ResourceManager::AddTextureNS(const std::string& p_path, const std::string& p_name)
{
    if (p_name.empty())
    {
        const std::string info("No name was entered for the creation of the texture. Please create it with a name");
        MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
        return nullptr;
    }

    for (auto texture : m_textures)
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

    std::shared_ptr<Texture> texture = Texture::LoadTexture(p_path, p_name);

    if (texture == nullptr)
    {
        const std::string
                error("ResourceManager::AddTexture(const std::string& p_path, const std::string& p_name): Could not load texture at "
                      + p_path + " because there was no object to be found at that path");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    texture->SetName(p_name);
    texture->SetPath(p_path);

    m_textures.push_back(texture);

    return texture;
}

std::shared_ptr<Texture> Engine::Managers::ResourceManager::GetTextureNS(const std::string& p_name)
{
    for (auto texture : m_textures)
    {
        if (texture->GetName() == p_name)
            return texture;
    }

    const std::string error("The texture: " + p_name + " does not exist");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONERROR | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<Texture>> Engine::Managers::ResourceManager::GetAllTexturesNS()
{
    return m_textures;
}

std::shared_ptr<PixelShader> Engine::Managers::ResourceManager::AddPixelShaderNS(const std::string& p_path, const std::string& p_name)
{
    if (p_name.empty())
    {
        const std::string info("No name was entered for the creation of the pixel shader. Please create it with a name");
        MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
        return nullptr;
    }

    for (auto pShader : m_pixelShaders)
    {
        if (pShader->GetPath() == p_path)
        {
            const std::string info("The pixel shader located at " + p_path + " is already loaded and will be returned");
            MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
            return pShader;
        }

        if (pShader->GetName() == p_name)
        {
            const std::string info("The name '" + p_name + "' is already in use, please change the name");
            MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
            return nullptr;
        }
    }

    std::shared_ptr<PixelShader> pShader = PixelShader::LoadShader(p_path, p_name);

    pShader->GenerateConstantBuffer();

    if (pShader == nullptr)
    {
        const std::string error{ "ResourceManager::AddPixelShader(const std::string& p_path, const std::string& p_name): Could not load pixel shader at "
                                + p_path + " because there was no object to be found at that path" };
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    pShader->SetName(p_name);
    pShader->SetPath(p_path);

    m_pixelShaders.push_back(pShader);

    return pShader;
}


std::shared_ptr<PixelShader> Engine::Managers::ResourceManager::GetPixelShaderNS(const std::string& p_name)
{
    for (auto pShader : m_pixelShaders)
    {
        if (pShader->GetName() == p_name)
            return pShader;
    }

    const std::string error("The pixel shader: " + p_name + " does not exist");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONERROR | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<PixelShader>> Engine::Managers::ResourceManager::GetAllPixelShadersNS()
{
    return m_pixelShaders;
}

std::shared_ptr<VertexShader> Engine::Managers::ResourceManager::AddVertexShaderNS(const std::string& p_path, const std::string& p_name)
{
    if (p_name.empty())
    {
        const std::string info("No name was entered for the creation of the vertex shader. Please create it with a name");
        MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
        return nullptr;
    }

    for (auto vShader : m_vertexShaders)
    {
        if (vShader->GetPath() == p_path)
        {
            const std::string info{ "The vertex shader located at " + p_path + " is already loaded and will be returned" };
            MessageBox(nullptr, info.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
            return vShader;
        }

        if (vShader->GetName() == p_name)
        {
            const std::string info{ "The name '" + p_name + "' is already in use, please change the name" };
            MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
            return nullptr;
        }
    }

    std::shared_ptr<VertexShader> vShader = VertexShader::LoadShader(p_path, p_name);

    vShader->GenerateConstantBuffer();

    if (vShader == nullptr)
    {
        const std::string
                error("ResourceManager::AddVertexShader(const std::string& p_path, const std::string& p_name): Could not load vertex shader at "
                      + p_path + " because there was no object to be found at that path");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    vShader->SetName(p_name);
    vShader->SetPath(p_path);

    m_vertexShaders.push_back(vShader);

    return vShader;
}


std::shared_ptr<VertexShader> Engine::Managers::ResourceManager::GetVertexShaderNS(const std::string& p_name)
{
    for (auto vShader : m_vertexShaders)
    {
        if (vShader->GetName() == p_name)
            return vShader;
    }

    const std::string error("The vertex shader: " + p_name + " does not exist");
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONERROR | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<VertexShader>> Engine::Managers::ResourceManager::GetAllVertexShadersNS()
{
    return m_vertexShaders;
}

std::shared_ptr<Material> Engine::Managers::ResourceManager::CreateMaterialNS(const std::string& p_name, const std::string& p_pixelShaderName,
                                                            const std::string& p_vertexShaderName, const std::string& p_textureName)
{
    if (p_name.empty())
    {
        const std::string info("No name was entered for the creation of the material. Please create it with a name");
        MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
        return nullptr;
    }

    //TODO when serialization is complete: export the new material in a file
    for (auto material : m_materials)
    {
        if (material->GetName() == p_name)
        {
            const std::string info("The name '" + p_name + "' is already in use, please change the name");
            MessageBox(nullptr, info.c_str(), "Error", MB_ICONERROR | MB_OK);
            return nullptr;
        }
    }

    if (GetPixelShaderNS(p_pixelShaderName) == nullptr)
    {
        const std::string error("The pixel shader you provided does not exist");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    if (GetVertexShaderNS(p_vertexShaderName) == nullptr)
    {
        const std::string error("The vertex shader you provided does not exist");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    if (p_textureName.length() > 0 && GetTextureNS(p_textureName) == nullptr)
    {
        const std::string error("The texture you provided does not exist");
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    std::shared_ptr<Material> material = Material::CreateMaterial(GetPixelShaderNS(p_pixelShaderName), GetVertexShaderNS(p_vertexShaderName));

    if (material == nullptr)
    {
        const std::string error{ "There was an error in the creation of the material"};
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    if (p_textureName.length() > 0)
        material->SetTexture(GetTextureNS(p_textureName));

    material->SetName(p_name);

    m_materials.push_back(material);

    return material;
}

std::shared_ptr<Material> Engine::Managers::ResourceManager::GetMaterialNS(const std::string& p_name)
{
    for (auto material : m_materials)
    {
        if (material->GetName() == p_name)
            return material;
    }

    const std::string error{ "The material: " + p_name + " does not exist" };
    MessageBox(nullptr, error.c_str(), "Error", MB_ICONERROR | MB_OK);
    return nullptr;
}

std::vector<std::shared_ptr<Material>> Engine::Managers::ResourceManager::GetAllMaterialsNS()
{
    return m_materials;
}

void Engine::Managers::ResourceManager::SerializeNS()
{
    std::ofstream outfile("SaveFiles/resource.IDResources");

    //Model
    for (auto& model : GetAllModelsNS())
    {
        if (!model->GetPath().empty())
            model->Serialize(outfile);
    }

    //Texture
    for (auto& texture : GetAllTexturesNS())
    {
        if (!texture->GetPath().empty())
            texture->Serialize(outfile);
    }

    //Pixel Shader
    for (auto& ps : GetAllPixelShadersNS())
    {
        if (ps->GetName() != "defaultPS" && !ps->GetPath().empty())
            ps->Serialize(outfile);
    }

    //Vertex Shader
    for (auto& vs : GetAllVertexShadersNS())
    {
        if (vs->GetName() != "defaultVS" && !vs->GetPath().empty())
            vs->Serialize(outfile);
    }

    //Material
    for (auto& material : GetAllMaterialsNS())
    {
        if (material->GetName() != "default" && material->GetName() != "RenderText")
            material->Serialize(outfile);
    }

    outfile.close();
}

void Engine::Managers::ResourceManager::DeserializeNS()
{
    std::ifstream inFile("SaveFiles/resource.IDResources");
    std::vector <std::string> lines;

    for (std::string line; std::getline(inFile, line); )
    {
        lines.push_back(line);
    }


    std::vector <std::string> words;

    for (auto& line : lines)
    {
        std::stringstream stringStream(line);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (words[0] == "MODEL")
        {
            AddModelNS(words[1], words[2]);
        }
        else if (words[0] == "TEXTURE")
        {
            AddTextureNS(words[1], words[2]);
        }
        else if (words[0] == "PS")
        {
            AddPixelShaderNS(words[1], words[2]);
        }
        else if (words[0] == "VS")
        {
            AddVertexShaderNS(words[1], words[2]);
        }
        else if (words[0] == "MATERIAL")
        {
            if (words[4] == "NO_TEXTURE" || words[4] == "NoName")
                CreateMaterialNS(words[1], words[2], words[3]);
            else
                CreateMaterialNS(words[1], words[2], words[3], words[4]);

            Vector3F color;
            color.x = std::stof(words[5]);
            color.y = std::stof(words[6]);
            color.z = std::stof(words[7]);
            GetMaterial(words[1])->SetColor(color);
        }

        words.clear();
    }

    inFile.close();
}
