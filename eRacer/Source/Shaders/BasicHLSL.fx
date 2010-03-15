//--------------------------------------------------------------------------------------
// File: BasicHLSL.fx
//
// 
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables, sent once per renderable
//--------------------------------------------------------------------------------------
const float4 g_MaterialAmbientColor;      // Material's ambient color
const float4 g_MaterialDiffuseColor;      // Material's diffuse color
const texture g_MeshTexture;              // Color texture for mesh
const float4 g_ColorTint;

const float4x4 g_ProjectionMatrix;
const float4x4 g_ViewMatrix;
const float4x4 g_WorldMatrix;


//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = ANISOTROPIC;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform bool bTexture)
{
    VS_OUTPUT Output;
      
    // Transform the position from object space to homogeneous projection space
    float4 vTmpPos = vPos;
    vTmpPos = mul( vTmpPos, g_WorldMatrix );
    vTmpPos = mul( vTmpPos, g_ViewMatrix );
    vTmpPos = mul( vTmpPos, g_ProjectionMatrix );
    Output.Position = vTmpPos;
    
    // Transform the normal from object space to world space
    float3 vNormalWorldSpace;
    float3 vTmpNorm = vNormal;
	vNormalWorldSpace = normalize(mul( vTmpNorm, (float3x3) g_WorldMatrix));
    
    //Global Lighting Information
    float3 lightDir = float3(1,1,1);
    float4 lightDiffuseColor = float4(0.9,0.9,0.9,1);
    float4 lightAmbientColor = float4(0.4,0.4,0.4,1);
    
    float3 calcDiffuse = g_MaterialDiffuseColor * (lightDiffuseColor * max(0,dot(vNormalWorldSpace, lightDir)));
    float3 calcAmbient = g_MaterialAmbientColor * lightAmbientColor; 
    
    Output.Diffuse = g_ColorTint;
    Output.Diffuse.rgb = Output.Diffuse.rgb * (calcDiffuse + calcAmbient);
    
    // Just copy the texture coordinate through
    if( bTexture ) 
        Output.TextureUV = vTexCoord0; 
    else
        Output.TextureUV = 0; 
    
    return Output;    
}

VS_OUTPUT RenderSceneFixedVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0,
                         uniform bool bTexture)
{
    VS_OUTPUT Output;
      
    // Transform the position from object space to homogeneous projection space
    float4 vTmpPos = vPos;
    vTmpPos = mul( vTmpPos, g_WorldMatrix );
    vTmpPos = mul( vTmpPos, g_ViewMatrix );
    vTmpPos = mul( vTmpPos, g_ProjectionMatrix );
    Output.Position = vTmpPos;
    
    // Transform the normal from object space to world space
    float3 vNormalWorldSpace;
    float3 vTmpNorm = vNormal;
	vNormalWorldSpace = normalize(mul( vTmpNorm, (float3x3) g_WorldMatrix));
    
    Output.Diffuse = g_ColorTint;
    
    // Just copy the texture coordinate through
    if( bTexture ) 
        Output.TextureUV = vTexCoord0; 
    else
        Output.TextureUV = 0; 
    
    return Output;    
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
    if( bTexture )
        Output.RGBColor = tex2D(MeshTextureSampler, In.TextureUV) * In.Diffuse;

    else
        Output.RGBColor = In.Diffuse;
    
    return Output;
}



//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTextureDefault
{
    pass P0
    {    
	  AlphaBlendEnable = true;
	  SrcBlend = srcalpha;
	  DestBlend = invsrcalpha;    
        VertexShader = compile vs_2_0 RenderSceneVS( true );
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTextureFixedLight
{
    pass P0
    {
	  AlphaBlendEnable = true;
	  SrcBlend = srcalpha;
	  DestBlend = invsrcalpha; 
	  VertexShader = compile vs_2_0 RenderSceneFixedVS( true );
	  PixelShader  = compile ps_2_0 RenderScenePS( true );
    }
}


technique RenderSceneNoTexture
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS( false );
        PixelShader  = compile ps_2_0 RenderScenePS( false ); // trivial pixel shader (could use FF instead if desired)
    }
}
