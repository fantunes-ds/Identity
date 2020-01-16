#pragma once
#include <Export.h>
#include <GPM/GPM.h>

namespace Engine::Geometry
{
	struct API_ENGINE Vertex
	{
		GPM::Vector2D m_textCoords{};
		GPM::Vector3D m_position{};
		GPM::Vector3D m_normal{};
	};
}
