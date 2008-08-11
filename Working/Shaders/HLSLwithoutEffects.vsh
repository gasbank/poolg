//-----------------------------------------------------------------------------
// File: HLSLwithoutEffects.vsh
//
// Desc: The effect file for the BasicHLSL sample.  It contains a vertex 
//		 shader which animates the vertices.
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 mWorldViewProj;  // World * View * Projection transformation
float4x4 mViewProj;
float fTime;			  // Time parameter. This keeps increasing


//-----------------------------------------------------------------------------
// Vertex shader output structure
//-----------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : POSITION;   // vertex position 
    float4 Tex0     : TEXCOORD0;
};


//-----------------------------------------------------------------------------
// Name: Ripple
// Type: Vertex shader                                      
// Desc: This shader ripples the vertices
//-----------------------------------------------------------------------------
VS_OUTPUT Ripple( in float4 vPosition : POSITION, in float2 vTex0 : TEXCOORD0 )
{
	VS_OUTPUT Output;
	
    float fSin, fCos;   
    float x = length( vPosition ) * sin( fTime ) * 15.0f;
    
    // This HLSL intrinsic computes returns both the sine and cosine of x
    sincos( x, fSin, fCos );

	// Change the y of the vertex position based on a function of time 
	// and transform the vertex into projection space. 
    
	//Output.Position = mul( float4( vPosition.x, fSin * 0.1f, vPosition.y, 1.0f ), mWorldViewProj );
	Output.Position = mul( float4( vPosition.x, vPosition.y, fSin, 1.0f ), mWorldViewProj );
    
    // Output the diffuse color as function of time and 
    // the vertex's object space position
    //Output.Diffuse = 0.5f - 0.5f * fCos;
	//Output.Diffuse = float4(1.0f, 0.5f, 0.0f, 1.0f);
	Output.Tex0 = float4(vTex0.x, vTex0.y, 0.0f, 1.0f);
    
    return Output;
}


