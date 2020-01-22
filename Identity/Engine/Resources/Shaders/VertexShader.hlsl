struct VSOut
{
    float4 col : Colour;
    float4 pos : SV_Position;
};


VSOut main( float2 pos : Position, float4 col : Colour )
{
    VSOut vso;
    vso.col = col;
    vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    return vso;
}