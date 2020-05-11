#pragma once
#include <Export.h>
#include <Objects/IObject.h>

namespace Engine::Rendering::Lights
{
    class API_ENGINE ILight: public Engine::Objects::IObject
    {
    public:
        struct LightData
        {
            Vector4F position;
            Vector4F ambient;
            Vector4F diffuse;
            Vector4F specular;
            Vector4F color;
            float shininess;
            Vector3F padding;
        };
        ILight(LightData p_lightData) : m_lightData{ p_lightData }{}

        LightData& GetLightData() { return m_lightData; }

        virtual void Serialize(std::ostream& p_stream) = 0;
        virtual void Deserialize(std::vector<std::string>& p_block) = 0;

    private:
        virtual bool operator==(Rendering::Lights::ILight* p_other) = 0;

        LightData m_lightData{};
    };
}
