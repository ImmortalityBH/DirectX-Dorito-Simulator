
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

Texture2D doritoTexture;
SamplerState samplerState;

float4 main(VS_OUT vso) : SV_TARGET
{
	return doritoTexture.Sample(samplerState, vso.texCoord);
}