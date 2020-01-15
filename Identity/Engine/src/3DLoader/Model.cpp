#include <stdafx.h>
#include <3DLoader/Model.h>

void Engine::Model::AddMesh(const Mesh& p_mesh)
{
	m_meshes.emplace_back(std::make_shared<Mesh>(p_mesh));
}
