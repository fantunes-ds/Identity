#include <stdafx.h>
#include <3DLoader/ObjectElements/Model.h>
#include <3DLoader/ObjectElements/Mesh.h>

void Engine::ObjectElements::Model::AddMesh(std::shared_ptr<Engine::ObjectElements::Mesh> p_mesh)
{
	m_meshes.emplace_back(p_mesh);
}
