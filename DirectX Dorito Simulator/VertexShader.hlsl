
cbuffer ConstantBuffer
{
	float4x4 transform;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

VS_OUT main( float3 pos : POSITION, float3 color : COLOR )
{
	VS_OUT vso;
	//vso.pos = mul(float4(pos.x,pos.y,pos.z, 1.0f), transform);
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
	vso.color = color;
	return vso;
}
