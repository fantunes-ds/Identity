#include <stdafx.h>
#include <3DLoader/ObjectLoader.h>
#include <3DLoader/ObjectElements/Transform.h>
#include "Containers/TransformContainer.h"

std::shared_ptr<Engine::ObjectElements::Model> Engine::ObjectLoader::LoadModel(const std::string& p_file)
{
    Engine::ObjectElements::Model model;

    Assimp::Importer importer;

    const aiScene* m_scene = importer.ReadFile(p_file,
        aiProcess_CalcTangentSpace
        | aiProcess_OptimizeGraph
        | aiProcess_Triangulate
        | aiProcess_SortByPType
        | aiProcess_GenNormals
        | aiProcess_FixInfacingNormals
        | aiProcess_GenUVCoords
        | aiProcess_MakeLeftHanded
        | aiProcess_FlipUVs
    );

    if (m_scene == nullptr)
    {
        std::cout << "Assimp error:" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    //Load Meshes
    for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i)
    {
        model.AddMesh(LoadMesh(m_scene->mMeshes[i]));
    }

    ParseTransforms(model, m_scene->mRootNode);

    return std::make_shared<Engine::ObjectElements::Model>(model);
}

void Engine::ObjectLoader::ParseTransforms(ObjectElements::Model& p_model, aiNode* p_node)
{
    ObjectElements::Transform transform{};

    GPM::Matrix4F mat
    {
        p_node->mTransformation.a1, p_node->mTransformation.a2, p_node->mTransformation.a3, p_node->mTransformation.a4,
        p_node->mTransformation.b1, p_node->mTransformation.b2, p_node->mTransformation.b3, p_node->mTransformation.b4,
        p_node->mTransformation.c1, p_node->mTransformation.c2, p_node->mTransformation.c3, p_node->mTransformation.c4,
        p_node->mTransformation.d1, p_node->mTransformation.d2, p_node->mTransformation.d3, p_node->mTransformation.d4
    };

    transform.SetTransformMatrix(mat);

    int32_t id = Containers::TransformContainer::AddTransform(std::make_shared<ObjectElements::Transform>(transform));
    p_model.GetMeshes()[p_node->mMeshes[0]]->SetTransform(id);

    for (size_t i = 0; i < p_node->mNumChildren; ++i)
    {
        ParseTransforms(p_model, p_node->mChildren[i]);
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

        vertex.m_position = GPM::Vector3D(vert.x, vert.y, -vert.z);
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

