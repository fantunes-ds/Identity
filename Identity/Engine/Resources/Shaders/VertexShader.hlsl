struct VSOut
{
    float4 col : Colour;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    matrix tranform;
};

VSOut main( float3 pos : Position)
{
    VSOut vso;
    vso.col = float4(1.0f, 0.5f, 0.5f, 1.0f);
    vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), tranform);
    return vso;
}