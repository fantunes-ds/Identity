#pragma once
#include <Export.h>
#include <map>
#include <memory>
#include <Systems/ISystem.h>

namespace Engine::Rendering
{
	class ILight;
}

namespace Engine::Systems
{
	class API_ENGINE LightSystem: public ISystem
	{
	public:
		~LightSystem() = default;

		static LightSystem* GetInstance();
		
	private:
		LightSystem() = default;

		static inline LightSystem* m_instance = nullptr;
		std::map<int32_t, std::shared_ptr<Rendering::ILight>> m_lights;
		
	};
}
