struct VSOut
{
    float4 col : Colour;
    float4 pos : SV_Position;
};

cbuffer CBuf
{
    row_major matrix tranform;
};

VSOut main( float2 pos : Position, float4 col : Colour )
{
    VSOut vso;
    vso.col = col;
    vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), tranform);
    return vso;
}