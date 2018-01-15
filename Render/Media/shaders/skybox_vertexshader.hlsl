cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float3 CameraPos;
}
struct VS_INPUT
{
	float4 Pos :POSITION;
	float2 Tex :TEXCOORD0;
};
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
	float3 dir : TEXCOORD1;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	float4 camera=mul(float4(0,0,0,1),World);
	output.Pos = mul(input.Pos, World);
	output.dir=(output.Pos-camera).xyz;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
    output.Pos=output.Pos.xyww;
	return output;
}