struct PS_IN
{
	float4 pos : SV_POSITION;
    float3 nor : NORMAL; // �@��
	float4 color : COLOR;
	float2 uv : TEXTURE0;
};

// Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
// Texture2D Texture : register(t0, space0);

// Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�
// SamplerState Sampler : register(s0, space0);

float4 main(PS_IN input) : SV_Target
{
    return input.color; // ���_�J���[�����̂܂܏o��
}
