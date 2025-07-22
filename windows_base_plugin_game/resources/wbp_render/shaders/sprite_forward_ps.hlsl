struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXTURE0;
};

Texture2D txDiffuse : register(t0, space0);
SamplerState samLinear : register(s0, space0);

float4 main(PS_IN input) : SV_Target
{
    // float4 output = txDiffuse.Sample(samLinear, input.uv);
    // return output;

    return (1.0f, 1.0f, 1.0f, 1.0f); // White color for debugging
}
