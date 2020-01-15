#pragma once
#include <Export.h>
#include <3DLoader/Mesh.h>

namespace Engine
{
	class API_ENGINE Model
	{
	public:
		Model() = default;
		~Model() = default;

		void AddMesh(const Mesh& p_mesh);
		
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}
