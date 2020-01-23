struct VSOut
{
    float4 col : Colour;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    matrix tranform;
};

VSOut main( float3 pos : Position, float4 col : Colour )
{
    VSOut vso;
    vso.col = col;
    vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f), tranform);
    return vso;
}