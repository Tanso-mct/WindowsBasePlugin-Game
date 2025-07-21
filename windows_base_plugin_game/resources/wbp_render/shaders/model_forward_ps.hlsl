struct PS_IN
{
	float4 pos : SV_POSITION;
    float3 nor : NORMAL; // 法線
	float4 color : COLOR;
	float2 uv : TEXTURE0;
};

// Textureをスロット0の0番目のテクスチャレジスタに設定
// Texture2D Texture : register(t0, space0);

// Samplerをスロット0の0番目のサンプラレジスタに設定
// SamplerState Sampler : register(s0, space0);

float4 main(PS_IN input) : SV_Target
{
    return input.color; // 頂点カラーをそのまま出力
}
