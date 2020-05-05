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

void Lights::DirectionalLight::Serialize(std::ostream& p_stream)
{
    p_stream << typeid(*this).name() << " " << std::to_string(m_id) << "\n{\n" <<
        "   m_lightData\n   {\n" <<
        "       shininess " << m_lightData.shininess << "\n" <<
        "       padding " << m_lightData.padding.x << " " << m_lightData.padding.y << " " << m_lightData.padding.z << "\n" <<
        "       position " << m_lightData.position.x << " " << m_lightData.position.y << " " << m_lightData.position.z << " " << m_lightData.position.w << "\n" <<
        "       ambient " << m_lightData.ambient.x << " " << m_lightData.ambient.y << " " << m_lightData.ambient.z << " " << m_lightData.ambient.w << "\n" <<
        "       diffuse " << m_lightData.diffuse.x << " " << m_lightData.diffuse.y << " " << m_lightData.diffuse.z << " " << m_lightData.diffuse.w << "\n" <<
        "       specular " << m_lightData.specular.x << " " << m_lightData.specular.y << " " << m_lightData.specular.z << " " << m_lightData.specular.w << "\n" <<
        "       color " << m_lightData.color.x << " " << m_lightData.color.y << " " << m_lightData.color.z << " " << m_lightData.color.w << "\n" <<
        "   }\n" << "}\n";
}

void Lights::DirectionalLight::Deserialize(std::istream& p_stream)
{
    std::vector < std::string> lines;
    std::vector < std::string> words;

    for (std::string line; std::getline(p_stream, line); )
    {
        lines.push_back(line);
        /*std::stringstream str(line);
        float x, y, z = std::atoi(line.c_str());
        //float x, y, z;
        sscanf_s(line.c_str(), "x : %f y : %f z : &f, ", &x, &y, &z);
        std::string s(std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "\n");
        OutputDebugString(s.c_str());*/
    }

    int lineNumber = 0;

    for (auto line: lines)
    {
        std::stringstream stringStream(line);

        do
        {
            std::string word;
            stringStream >> word;
            words.push_back(word);
            OutputDebugString((word + "\n").c_str());
        } while (stringStream);

        if (words[0] == "shininess")
        {
            m_lightData.shininess = 0.0f;
            m_lightData.shininess = std::stof(words[1]);
        }
        else if (words[0] == "padding")
        {
            m_lightData.padding.x = std::stof(words[1]);
            m_lightData.padding.y = std::stof(words[2]);
            m_lightData.padding.z = std::stof(words[3]);
        }
        else if (words[0] == "position")
        {
            m_lightData.position.x = std::stof(words[1]);
            m_lightData.position.y = std::stof(words[2]);
            m_lightData.position.z = std::stof(words[3]);
            m_lightData.position.w = std::stof(words[4]);
        }
        else if (words[0] == "ambient")
        {
            m_lightData.ambient.x = std::stof(words[1]);
            m_lightData.ambient.y = std::stof(words[2]);
            m_lightData.ambient.z = std::stof(words[3]);
            m_lightData.ambient.w = std::stof(words[4]);
        }
        else if (words[0] == "diffuse")
        {
            m_lightData.diffuse.x = std::stof(words[1]);
            m_lightData.diffuse.y = std::stof(words[2]);
            m_lightData.diffuse.z = std::stof(words[3]);
            m_lightData.diffuse.w = std::stof(words[4]);
        }
        else if (words[0] == "specular")
        {
            m_lightData.specular.x = std::stof(words[1]);
            m_lightData.specular.y = std::stof(words[2]);
            m_lightData.specular.z = std::stof(words[3]);
            m_lightData.specular.w = std::stof(words[4]);
        }
        else if (words[0] == "color")
        {
            m_lightData.color.x = std::stof(words[1]);
            m_lightData.color.y = std::stof(words[2]);
            m_lightData.color.z = std::stof(words[3]);
            m_lightData.color.w = std::stof(words[4]);
        }

        words.clear();
        ++lineNumber;
        //OutputDebugString((line + "\n").c_str());
    }
}
