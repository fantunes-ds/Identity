Texture2D shaderTexture;
SamplerState SampleType;

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

float4 main(VS_OUT f_in) : SV_TARGET
{
    // ambient calculations
    float3 ambient = light.ambient.rgb * light.color.rgb;
    
    //diffuse
    f_in.norm = normalize(f_in.norm);
    float3 lightDir = normalize(light.position.rgb - f_in.worldPos);
    float diff = max(dot(f_in.norm, lightDir), 0.0);
    float3 diffuse = diff * light.color.rgb;
    
    //specular
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
    float3 specular = light.specular.rgb * spec * light.color.rgb;

    f_in.vertexColor = float4(materialColor, 1);
    //f_in.vertexColor = float4(1, 0, 0, 1);
    if (textureState != 0.0f)
    {
        f_in.vertexColor = shaderTexture.Sample(SampleType, f_in.tex) * f_in.vertexColor;
    }
    f_in.vertexColor = float4(ambient + diffuse + specular, 1) * f_in.vertexColor;
    return f_in.vertexColor;
}
