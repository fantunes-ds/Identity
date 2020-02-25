struct lightSource
{
    float4 position;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 color;
    float shininess;
    float3 padding;
};
cbuffer CBuf
{
    lightSource light;
    float padding;
    float3 cameraPos;
    float padding2;
};

struct VS_OUT
{
    float4 vertexColor : Colour;
    float3 pos : Position;
    float3 worldPos : WPos;
    float3 norm : Normal;
};

static bool blinn = true;

float4 main(VS_OUT f_in) : SV_TARGET
{
    f_in.norm = normalize(f_in.norm);
    
    float3 ambient = light.ambient * light.color;
    float3 lightDir = normalize(light.position.rgb - f_in.worldPos);
    
    float diff = max(dot(f_in.norm, lightDir), 0.0);
    float3 diffuse = diff * light.color;
    
    float3 viewDir = normalize(cameraPos - f_in.worldPos);
    float spec = 0.0f;
    if (blinn)
    {
        float3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(f_in.norm, halfwayDir), 0.0f), light.shininess);
    }
    else
    {
        float3 reflectDir = reflect(-lightDir, f_in.norm);
        spec = pow(max(dot(cameraPos, reflectDir), 0.0f), light.shininess);
    }
    float3 specular = light.specular * spec * light.color;
    f_in.vertexColor = float4(ambient + diffuse + specular, 1) * f_in.vertexColor;
    return f_in.vertexColor;
}