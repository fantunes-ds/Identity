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
    private:
        virtual bool operator==(Rendering::Lights::ILight* p_other) = 0;

        LightData m_lightData{};
    };
}
