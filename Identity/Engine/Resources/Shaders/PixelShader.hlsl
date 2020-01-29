struct lightSource
{
    float3 position;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float3 direction;
};

cbuffer CBuf
{
    lightSource light;
    float shininess;
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
    float3 diffuse = diff * float3(1.0f, 1.0f, 1.0f);
    
    float3 viewDir = normalize(float3(0.0f, 0.0f, 0.0f) - f_in.worldPos);
    float3 reflectDir = reflect(-lightDir, f_in.norm);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float3 specular = light.specular * spec * float3(1.0f, 1.0f, 1.0f);
    
    f_in.vertexColor *= float4(light.ambient + diffuse + specular, 1) * f_in.vertexColor;
    return f_in.vertexColor;
}