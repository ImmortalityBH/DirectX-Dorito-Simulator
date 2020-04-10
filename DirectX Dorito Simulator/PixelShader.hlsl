
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

float4 main(VS_OUT vso) : SV_TARGET
{
	return float4(vso.color, 1.0f);
}