#include <stdafx.h>
#include <3DLoader/ObjectLoader.h>
#include <Components/Transform.h>
#include <Scene/SceneGraph/SceneNode.h>
#include <Tools/ASSIMP/ASSIMPConversion.h>
#include <Systems/TransformSystem.h>

std::shared_ptr<Engine::ObjectElements::Model> Engine::ObjectLoader::LoadModel(const std::string& p_file)
{
    Engine::ObjectElements::Model model;
    std::shared_ptr<Scene::SceneNode> rootSceneNode = std::make_shared<Scene::SceneNode>();

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(p_file,
        aiProcess_CalcTangentSpace
        | aiProcess_OptimizeGraph
 // | aiProcess_OptimizeMeshes
        | aiProcess_Triangulate
        | aiProcess_SortByPType
        | aiProcess_GenNormals
        | aiProcess_FixInfacingNormals
        | aiProcess_GenUVCoords
        | aiProcess_MakeLeftHanded
        | aiProcess_FlipUVs
        | aiProcess_ValidateDataStructure
        | aiProcess_FindInvalidData
    );

    if (scene == nullptr)
    {
        const std::string error("Assimp error loading file at " + p_file + ": " + importer.GetErrorString());
        MessageBox(nullptr, error.c_str(), "Error", MB_ICONWARNING | MB_OK);
        return nullptr;
    }

    //Load Meshes
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        auto mesh = LoadMesh(scene->mMeshes[i]);
        model.AddMesh(mesh);
    }

    model.SetRootNode(rootSceneNode);
    ParseForNodes(scene->mRootNode, rootSceneNode, model);

    //If Assimp didn't create a node hierarchy while parsing
    if (rootSceneNode->GetChildren().empty())
    {
        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            rootSceneNode->AddChild(std::make_shared<Scene::SceneNode>(model.GetMeshes()[i]));
        }
    }


    return std::make_shared<Engine::ObjectElements::Model>(model);
}

void Engine::ObjectLoader::ParseForNodes(const aiNode* p_assimpNode, std::shared_ptr<Scene::SceneNode> p_sceneNode, ObjectElements::Model& p_model)
{
    if (!p_sceneNode->IsRoot())
    {
        // auto newTransform = std::make_shared<Components::Transform>();
        int32_t id = Containers::TransformSystem::AddTransform();
        GPM::Matrix4F local = Tools::ASSIMP::ASSIMPConversion::Mat4x4ToGPM(p_assimpNode->mTransformation);
        Containers::TransformSystem::GetTransform(id)->SetLocalTransformMatrix(local);
        // newTransform->SetLocalTransformMatrix(local);

        p_sceneNode->SetTransform(id);
    }
    
    p_sceneNode->SetName(p_assimpNode->mName.C_Str()); 

    for (unsigned int i = 0; i < p_assimpNode->mNumChildren; ++i)
    {
        //Will throw exception if loaded object was not properly exported (ie. true number of meshes is not the same as specified during export).
        auto newChild = std::make_shared<Scene::SceneNode>(p_model.GetMeshes()[p_assimpNode->mMeshes[i]]);

        p_sceneNode->AddChild(newChild);
        ParseForNodes(p_assimpNode->mChildren[i], newChild, p_model);
    }
}

std::shared_ptr<Engine::ObjectElements::Mesh> Engine::ObjectLoader::LoadMesh(aiMesh* p_assimpMesh)
{
    std::vector<Engine::Geometry::Vertex> vertices;
    std::vector<unsigned short> indices;
    int offset = 0;

    for (unsigned int vertIdx = 0u; vertIdx < p_assimpMesh->mNumVertices; vertIdx++)
    {
        Engine::Geometry::Vertex vertex;

        const aiVector3D vert = p_assimpMesh->mVertices[vertIdx];
        const aiVector3D norm = p_assimpMesh->mNormals[vertIdx];

        if (p_assimpMesh->HasTextureCoords(0))
        {
            const aiVector3D UV = p_assimpMesh->mTextureCoords[0][vertIdx];
            vertex.m_textCoords = GPM::Vector2D(UV.x, UV.y);
        }

        vertex.m_position = GPM::Vector3D(vert.x, vert.y, vert.z);
        vertex.m_normal = GPM::Vector3D(norm.x, norm.y, norm.z);

        vertices.emplace_back(vertex);
    }

    //Load Indices
    for (unsigned int faceIdx = 0; faceIdx < p_assimpMesh->mNumFaces; faceIdx++)
    {
        if (p_assimpMesh->mFaces[faceIdx].mNumIndices == 3)
        {
            indices.push_back(p_assimpMesh->mFaces[faceIdx].mIndices[0] + offset);
            indices.push_back(p_assimpMesh->mFaces[faceIdx].mIndices[1] + offset);
            indices.push_back(p_assimpMesh->mFaces[faceIdx].mIndices[2] + offset);
        }
    }
    offset += p_assimpMesh->mNumVertices;

    return std::make_shared<Engine::ObjectElements::Mesh>(vertices, indices);
}

