#pragma once
#include <Components/IComponent.h>
#include <Rendering/Lights/ILight.h>

namespace Engine::Rendering::Lights
{
    class DirectionalLight : public ILight
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

        DirectionalLight() = default;
        DirectionalLight(const Vector4F& p_position, const Vector4F&  p_ambient, const Vector4F& p_diffuse, const Vector4F& p_specular,
              const Vector4F& p_direction, const Vector4F& p_color, const float& p_shininess);
        DirectionalLight(const LightData& p_lightData);


        inline LightData& GetLightData() { return m_lightData; }

        bool operator==(Rendering::Lights::ILight* p_other) override;

    private:
        LightData m_lightData{};
    };
}
