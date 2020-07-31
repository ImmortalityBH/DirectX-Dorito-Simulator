struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

Texture2D doritoTexture;
SamplerState samplerState;

float4 main(VS_OUT vso) : SV_TARGET
{
	float4 diffuse = doritoTexture.Sample(samplerState, vso.texCoord);
	
	clip(diffuse.a - .25);
	return diffuse;
}