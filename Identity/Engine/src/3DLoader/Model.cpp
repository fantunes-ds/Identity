#include <stdafx.h>
#include <3DLoader/Model.h>

void Engine::Model::AddMesh(std::shared_ptr<Mesh> p_mesh)
{
	m_meshes.emplace_back(p_mesh);
}
