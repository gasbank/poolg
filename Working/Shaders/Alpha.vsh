
float4x4 mWorldViewProj;
float fTime; // Time parameter. This keeps increasing

struct VS_OUTPUT
{
	float4 Position		: POSITION;
	float4 Tex0			: TEXCOORD0;
	float4 Diffuse		: COLOR0;
};


VS_OUTPUT Alpha( in float4 vPosition : POSITION, in float2 vTex0 : TEXCOORD0, in float4 vColor : COLOR0 )
{
	VS_OUTPUT Output;
	Output.Position = mul( vPosition, mWorldViewProj );
	Output.Diffuse.r = 0.0f;
	Output.Diffuse.g = 0.0f;
	Output.Diffuse.b = 0.0f;
	Output.Diffuse.a = abs( sin( fTime ) ) / 2;
	Output.Tex0 = float4( vTex0.x, vTex0.y, sin( fTime ), 1.0f );
    return Output;
}
