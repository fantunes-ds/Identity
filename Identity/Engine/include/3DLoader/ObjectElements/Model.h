#pragma once
#include <Export.h>
#include <3DLoader/ObjectElements/Mesh.h>

namespace Engine::ObjectElements
{
	class API_ENGINE Model
	{
	public:
		Model() = default;
		~Model() = default;

		void AddMesh(std::shared_ptr<Mesh> p_mesh);
		
		std::vector<std::shared_ptr<Mesh>> m_meshes;
	};
}