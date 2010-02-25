/* STARFIELD */

const float4x4    PMatrix;
const float4x4    V1Matrix;
const float4x4    V2Matrix;
const float3  camPos;
const float   SIZE;
struct STARS_VS_OUTPUT
{
    float4 Position : POSITION;
    float4 RGBColor : COLOR0;
};
struct STARS_PS_OUTPUT
{
    float4 RGBColor : COLOR0;
};

STARS_VS_OUTPUT StarsVertex(
    float4 vPos   : POSITION, 
    float4 vColor : COLOR0
)
{
    STARS_VS_OUTPUT Output;
    
    vPos.x = camPos.x + ((vPos.x - camPos.x + 1e5) % (2*SIZE)) - SIZE;
    vPos.y = camPos.y + ((vPos.y - camPos.y + 1e5) % (2*SIZE)) - SIZE;
    vPos.z = camPos.z + ((vPos.z - camPos.z + 1e5) % (2*SIZE)) - SIZE;
    
    if (vColor.a)
      Output.Position = mul(vPos, V1Matrix);
    else
      Output.Position = mul(vPos, V2Matrix);
    Output.Position = mul(Output.Position, PMatrix);
    Output.RGBColor = vColor;
    Output.RGBColor.a = 1;
    return Output;
}

STARS_PS_OUTPUT StarsPixel(
    STARS_PS_OUTPUT In
)
{
    STARS_PS_OUTPUT Output;
    Output.RGBColor = In.RGBColor;
    return Output;
}


technique Starfield
{
    pass P0
    {          
        VertexShader = compile vs_2_0 StarsVertex();
        PixelShader  = compile ps_2_0 StarsPixel();
    }
}
