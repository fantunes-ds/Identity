#include <stdafx.h>

#include <Rendering/Lights/DirectionalLight.h>

using namespace Engine::Rendering;


Lights::DirectionalLight::DirectionalLight() : ILight{LightData{}}
{
}

Lights::DirectionalLight::DirectionalLight(const Vector4F& p_position, const Vector4F&  p_ambient,
                                           const Vector4F& p_diffuse, const Vector4F&   p_specular,
                                           const Vector4F& p_direction, const Vector4F& p_color,
                                           const float& p_shininess) : ILight{ LightData{p_position, p_ambient, p_diffuse, p_specular, p_color, p_shininess} }
{
    // m_lightData = { p_position, p_ambient, p_diffuse, p_specular, p_color, p_shininess };
}

Lights::DirectionalLight::DirectionalLight(const LightData& p_lightData) : ILight{p_lightData}
{
    // m_lightData = p_lightData;
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

void Lights::DirectionalLight::Serialize(std::ostream& p_stream)
{
    p_stream << typeid(*this).name() << "\n{\n" <<
        "   m_lightData\n   {\n" <<
        "       shininess " << GetLightData().shininess << "\n" <<
        "       padding " << GetLightData().padding << "\n" <<
        "       position " << GetLightData().position << "\n" <<
        "       ambient " << GetLightData().ambient << "\n" <<
        "       diffuse " << GetLightData().diffuse << "\n" <<
        "       specular " << GetLightData().specular << "\n" <<
        "       color " << GetLightData().color << "\n" <<
        "   }\n" << "}\n";
}

void Lights::DirectionalLight::Deserialize(std::vector<std::string>& p_block)
{
    std::vector <std::string> words;

    for (auto& line: p_block)
    {
        std::stringstream stringStream(line);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
        } while (stringStream);

        if (words[1] == "Engine::Rendering::Lights::DirectionalLight")
        {
            m_id = std::stoi(words[2]);
        }
        else if (words[0] == "shininess")
        {
            GetLightData().shininess = 0.0f;
            GetLightData().shininess = std::stof(words[1]);
        }
        else if (words[0] == "padding")
        {
            GetLightData().padding.x = std::stof(words[1]);
            GetLightData().padding.y = std::stof(words[2]);
            GetLightData().padding.z = std::stof(words[3]);
        }
        else if (words[0] == "position")
        {
            GetLightData().position.x = std::stof(words[1]);
            GetLightData().position.y = std::stof(words[2]);
            GetLightData().position.z = std::stof(words[3]);
            GetLightData().position.w = std::stof(words[4]);
        }
        else if (words[0] == "ambient")
        {
            GetLightData().ambient.x = std::stof(words[1]);
            GetLightData().ambient.y = std::stof(words[2]);
            GetLightData().ambient.z = std::stof(words[3]);
            GetLightData().ambient.w = std::stof(words[4]);
        }
        else if (words[0] == "diffuse")
        {
            GetLightData().diffuse.x = std::stof(words[1]);
            GetLightData().diffuse.y = std::stof(words[2]);
            GetLightData().diffuse.z = std::stof(words[3]);
            GetLightData().diffuse.w = std::stof(words[4]);
        }
        else if (words[0] == "specular")
        {
            GetLightData().specular.x = std::stof(words[1]);
            GetLightData().specular.y = std::stof(words[2]);
            GetLightData().specular.z = std::stof(words[3]);
            GetLightData().specular.w = std::stof(words[4]);
        }
        else if (words[0] == "color")
        {
            GetLightData().color.x = std::stof(words[1]);
            GetLightData().color.y = std::stof(words[2]);
            GetLightData().color.z = std::stof(words[3]);
            GetLightData().color.w = std::stof(words[4]);
        }

        words.clear();
    }
}
