
SamplerState samLinear : register(s0);
//Texture2D	textureMap  : register(t0);
TextureCube CubeMap : register(t0);
struct PS_INPUT
{
	float4 Pos: SV_POSITION;
	float2 Tex :TEXCOORD0;
	float3 dir :TEXCOORD1;
};
float4 main(PS_INPUT input) : SV_TARGET
{
	//return textureMap.Sample(samLinear, input.Tex);

	//return CubeMap.Sample(samLinear,input.dir.xyz);
	return CubeMap.Sample(samLinear,float3(input.dir.x,input.dir.y-0.3,input.dir.z));
	//return float4(input.dir.xyz*0.5+0.5,1);
	//return float4(1.0f,1.0f,0.0f,1.0f);
}
