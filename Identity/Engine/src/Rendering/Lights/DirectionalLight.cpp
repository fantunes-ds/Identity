#include <stdafx.h>

#include <Rendering/Lights/DirectionalLight.h>

using namespace Engine::Rendering;

Lights::DirectionalLight::DirectionalLight(const Vector4F& p_position, const Vector4F&  p_ambient,
                                           const Vector4F& p_diffuse, const Vector4F&   p_specular,
                                           const Vector4F& p_direction, const Vector4F& p_color,
                                           const float&    p_shininess) : m_lightData{p_position, p_ambient, p_diffuse,
                                                                                      p_specular, p_color, p_shininess} {}

Lights::DirectionalLight::DirectionalLight(const LightData& p_lightData)
{
    m_lightData = p_lightData;
}

bool Lights::DirectionalLight::operator==(ILight* p_other)
{
    if (DirectionalLight* other = dynamic_cast<DirectionalLight*>(p_other))
    {
        auto thisData  = GetLightData();
        auto otherData = other->GetLightData();


        if (thisData.shininess == otherData.shininess &&
            thisData.color == otherData.color &&
            thisData.ambient == otherData.ambient &&
            thisData.diffuse == otherData.diffuse &&
            thisData.position == otherData.position)
            return true;
    }

    return false;
}
