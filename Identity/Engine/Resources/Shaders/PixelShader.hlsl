struct lightSource
{
    float3 position;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float3 direction;
};
static float3 lightColor = float3(0.95f, 0.95f, 0.95f);
cbuffer CBuf
{
    lightSource light;
    float shininess;
    float3 cameraPos;
};

struct VS_OUT
{
    float4 vertexColor : Colour;
    float3 pos : Position;
    float3 worldPos : WPos;
    float3 norm : Normal;
};

float4 main(VS_OUT f_in) : SV_TARGET
{
    f_in.norm = normalize(f_in.norm);
    float3 lightDir = normalize(light.position - f_in.worldPos);
    
    float diff = max(dot(f_in.norm, lightDir), 0.0);
    float3 diffuse = diff * lightColor;
    
    float3 viewDir = normalize(cameraPos - f_in.worldPos);
    float3 reflectDir = reflect(-lightDir, f_in.norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float3 specular = light.specular * spec * lightColor;
    
    f_in.vertexColor *= float4(light.ambient + diffuse + specular, 1) * f_in.vertexColor;
    return f_in.vertexColor;
}