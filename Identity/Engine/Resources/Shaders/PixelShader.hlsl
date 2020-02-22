struct lightSource
{
    float4 position;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 direction;
    float4 color;
    float shininess;
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

float4 main(VS_OUT f_in) : SV_TARGET
{
    f_in.norm = normalize(f_in.norm);
    float3 lightDir = normalize(light.position.rgb - f_in.worldPos);
    
    float diff = max(dot(f_in.norm, lightDir), 0.0);
    float3 diffuse = diff * light.color;
    
    float3 viewDir = normalize(cameraPos - f_in.worldPos);
    float3 reflectDir = reflect(-lightDir, f_in.norm);
    
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), light.shininess);
    float3 specular = spec * light.color;
    
    f_in.vertexColor *= float4(light.ambient.rgb + diffuse + specular, 1) * f_in.vertexColor;
    return f_in.vertexColor;
}