#include <stdafx.h>
#include <3DLoader/ObjectLoader.h>

std::shared_ptr<Engine::Model> Engine::ObjectLoader::LoadModel(const std::string& p_file)
{
	std::vector<Geometry::Vertex> vertices;
	std::vector<uint32_t> indices;
	
	Engine::Model model;
	
	int offset = 0;

	Assimp::Importer m_importer;
	const aiScene* m_scene = m_importer.ReadFile(p_file,
		aiProcess_CalcTangentSpace
		| aiProcess_OptimizeGraph
		| aiProcess_Triangulate
		| aiProcess_SortByPType
		| aiProcess_GenNormals
		| aiProcess_FixInfacingNormals
		| aiProcess_GenUVCoords
		| aiProcess_FlipUVs
	);
	
	if (m_scene == nullptr)
	{
		puts("Mesh Path error");
		return nullptr;
	}
	//Load Meshes
	for (unsigned int i = 0; i < m_scene->mNumMeshes; ++i)
	{
		aiMesh* t_mesh = m_scene->mMeshes[i];

		//Load Vertices
		for (unsigned int vertIdx = 0u; vertIdx < t_mesh->mNumVertices; vertIdx++)
		{
			Geometry::Vertex vertex;

			aiVector3D vert = t_mesh->mVertices[vertIdx];
			aiVector3D norm = t_mesh->mNormals[vertIdx];

			if (t_mesh->HasTextureCoords(0))
			{
				aiVector3D UV = t_mesh->mTextureCoords[0][vertIdx];
				vertex.m_textCoords = GPM::Vector2D(UV.x, UV.y);
			}

			vertex.m_position = GPM::Vector3D(vert.x, vert.y, vert.z);
			vertex.m_normal = GPM::Vector3D(norm.x, norm.y, norm.z);

			vertices.push_back(vertex);
		}
		//Load Indices
		for (unsigned int faceIdx = 0; faceIdx < t_mesh->mNumFaces; faceIdx++)
		{
			if (t_mesh->mFaces[faceIdx].mNumIndices == 3)
			{
				indices.push_back(t_mesh->mFaces[faceIdx].mIndices[0] + offset);
				indices.push_back(t_mesh->mFaces[faceIdx].mIndices[1] + offset);
				indices.push_back(t_mesh->mFaces[faceIdx].mIndices[2] + offset);
			}
		}
		offset += t_mesh->mNumVertices;

		model.AddMesh(Mesh{ vertices, indices });
	}
	
	return std::make_shared<Model>(model);
}

Engine::ObjectLoader::ObjectLoader()
{
}

Engine::ObjectLoader::~ObjectLoader()
{
	delete m_instance;
}

Engine::ObjectLoader* Engine::ObjectLoader::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ObjectLoader();
	}

	return m_instance;
}

