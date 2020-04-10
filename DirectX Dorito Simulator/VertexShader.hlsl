
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

VS_OUT main( float4 pos : POSITION, float3 color : COLOR )
{
	VS_OUT vso;
	vso.pos = pos;
	vso.color = color;
	return vso;
}