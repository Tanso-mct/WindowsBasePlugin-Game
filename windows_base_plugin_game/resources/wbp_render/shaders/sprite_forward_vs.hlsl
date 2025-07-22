
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXTURE0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXTURE0;
};

cbuffer UIBuffer : register(b0) 
{
    float2 scale;
    float2 offset;
    float  zValue;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    float2 scaledPos = input.pos * scale + offset;
    output.pos = float4(scaledPos, zValue, 1.0f);
    output.uv = input.uv;
    return output;
}
