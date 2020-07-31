
cbuffer ConstantBuffer
{
	float4x4 WVP;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

VS_OUT main(float4 inPos : POSITION, float4 inColor : COLOR)
{
	VS_OUT vso;
	//vso.pos = mul(float4(pos.x,pos.y,pos.z, 1.0f), transform);
	vso.pos = mul(inPos, WVP);
	vso.color = inColor;
	return vso;
}