#include <stdafx.h>
#include <3DLoader/ObjectLoader.h>

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

    return std::make_shared<Engine::ObjectElements::Model>(model);
}

std::shared_ptr<Engine::ObjectElements::Mesh> Engine::ObjectLoader::LoadMesh(aiMesh* p_assimpMesh)
{
    std::vector<Engine::Geometry::Vertex> vertices;
    std::vector<unsigned short> indices;
    int offset = 0;

    for (unsigned int vertIdx = 0u; vertIdx < p_assimpMesh->mNumVertices; vertIdx++)
    {
        Engine::Geometry::Vertex vertex;

        aiVector3D vert = p_assimpMesh->mVertices[vertIdx];
        aiVector3D norm = p_assimpMesh->mNormals[vertIdx];

        if (p_assimpMesh->HasTextureCoords(0))
        {
            aiVector3D UV = p_assimpMesh->mTextureCoords[0][vertIdx];
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

