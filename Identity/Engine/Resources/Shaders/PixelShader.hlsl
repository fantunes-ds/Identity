Texture2D shaderTexture;
SamplerState SampleType;

struct lightSource
{
    float4 position;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
    float range;
    float intensity;
    float padding;
};

cbuffer CBuf
{
    lightSource lights[6];
    float4 ambientColor;
    float3 cameraPos;
    float textureState;
    float3 materialColor;
    float buffer;
};

struct VS_OUT
{
    float4 vertexColor : Colour;
    float3 pos : Position;
    float3 worldPos : WPos;
    float3 norm : Normal;
    float2 tex : TexCoord;
};

static bool blinn = true;

float3 CalculateLights(lightSource light, VS_OUT f_in);
float CalculateAttenuation(float distance, float range, float intensity);

float4 main(VS_OUT f_in) : SV_TARGET
{
    float3 fColor = (0, 0, 0);
    
    fColor = (ambientColor.rgb * ambientColor.w);
    
    for (int i = 0; i < 6; ++i)
        fColor += CalculateLights(lights[i], f_in);
    
    f_in.vertexColor = float4(materialColor, 1);
    if (textureState != 0.0f)
    {
        f_in.vertexColor = shaderTexture.Sample(SampleType, f_in.tex) * f_in.vertexColor;
    }
    f_in.vertexColor = float4(fColor, 1) * f_in.vertexColor;
    return f_in.vertexColor;
}

float3 CalculateLights(lightSource light, VS_OUT f_in)
{
    if (light.padding < 0)
        return (0, 0, 0);
    
    float3 fColor = (0, 0, 0);
    
    float distance = length(light.position.rgb - f_in.worldPos);
    float attenuation = CalculateAttenuation(distance, light.range, light.diffuse.w);
    
    // ambient calculations
    //fColor = (ambientColor.rgb * ambientColor.w);
    
    //diffuse
    f_in.norm = normalize(f_in.norm);
    float3 lightDir = normalize(light.position.rgb - f_in.worldPos);
    float diff = max(dot(f_in.norm, lightDir), 0.0);
    float3 diffuse = diff * (light.diffuse.rgb);
    fColor += (diffuse * attenuation);
    
    //specular
    float3 viewDir = normalize(cameraPos - f_in.worldPos);
    float spec = 0.0f;
    float3 specular = (0, 0, 0);
    
    float3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(f_in.norm, halfwayDir), 0.0f), light.shininess);
    specular = (light.specular.rgb * light.specular.w) * spec * light.diffuse.rgb;
    
    fColor += (specular * attenuation);
    
    return fColor;
}

float CalculateAttenuation(float distance, float range, float intensity)
{
    range *= 10;
    float attenuation = 0.f;
    float distanceOverRange = distance / range;
    float distanceOverRange2 = pow(distanceOverRange, 2);
    float distanceOverRange4 = pow(distanceOverRange2, 2);
    
    attenuation = pow(saturate(1.0f - distanceOverRange4), 2);
    attenuation /= pow(distance, 2) + 1.0f;
    //return attenuation * intensity;
    
    attenuation = max(1.0f / (distance) - 1.0f / (range), 0.0f);
    return attenuation * intensity;
}