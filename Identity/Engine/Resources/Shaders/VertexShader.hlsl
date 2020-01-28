struct VSOut
{
    float4 col : Colour;
    float4 pos : SV_Position;
    float3 worldPos : WPos;
    float3 norm : Normal;
};

cbuffer CBuf
{
    matrix model;
    matrix perspective;
};

VSOut main( float3 vpos : Position, float3 vnorm: Normal)
{
    VSOut vso;
    vso.col = float4(1.0f, 0.5f, 0.5f, 1.0f);
    vso.pos = mul(float4(vpos.x, vpos.y, vpos.z, 1.0f), mul(model, perspective));
    vso.worldPos = float4(mul(model, float4(vpos, 1.0f))).rgb;
    vso.norm = vnorm;
    return vso;
}