
cbuffer ConstantBuffer
{
	float4x4 WVP;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUT main( float4 inPos : POSITION, float3 inTexCoord : TEXCOORD )
{
	VS_OUT vso;
	//vso.pos = mul(float4(pos.x,pos.y,pos.z, 1.0f), transform);
	vso.pos = inPos;
	vso.texCoord = inTexCoord;
	return vso;
}
