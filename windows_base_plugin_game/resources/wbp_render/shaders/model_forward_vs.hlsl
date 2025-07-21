
struct VS_IN //���_�V�F�[�_���� INPUT LAYOUT
{
	float3 pos : POSITION0;
	float3 nor : NORMAL0;
	float4 color : COLOR0; //Diffuse�i�\�ʊg�U���F�j
	float2 uv : TEXTURE0;

    uint4 indices0 : BLENDINDICES0;
	uint4 indices1 : BLENDINDICES1;
	float4 weights0 : BLENDWEIGHT0;
	float4 weights1 : BLENDWEIGHT1;
};

struct VS_OUT //�s�N�Z���V�F�[�_�ւ̏o��
{
	float4 pos : SV_POSITION;
    float3 nor : NORMAL; // �@��
	float4 color : COLOR;
	float2 uv : TEXTURE0;
};

cbuffer ViewBuffer : register(b0)
{
	float4x4 View; // �r���[�ϊ��s��
}

cbuffer ProjectionBuffer : register(b1)
{
	float4x4 Projection; // �ˉe�ϊ��s��
}

cbuffer WorldBuffer : register(b2)
{
	float4x4 World; // ���[���h�ϊ��s��
}

cbuffer MeshBone : register(b3)
{
	float4x4 bones[2]; //�{�[���p�z��B�f�[�^���I�[�o�[���Ă��ϒ��ɂȂ�̂ő��v�B
	//�������P���ƌŒ蒷�ɂȂ��Ă��܂��̂łQ�ȏ�ɂ��鎖�B
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	output.pos = float4(input.pos, 1.0f);

	output.pos = mul(output.pos, World); // ���[�J�����W * ���[���h���W�ϊ��s��
	output.pos = mul(output.pos, View); // ���[���h���W * �r���[���W�ϊ��s��
	output.pos = mul(output.pos, Projection); // �r���[���W * �v���W�F�N�V�������W�ϊ��s��

    // �@�������[���h���W�n�ɕϊ�
    float4 normal = float4(input.nor, 0.0f);
    normal = mul(normal, World);

    output.nor = normal.xyz;

	output.color = input.color; // ���_�J���[
	output.uv = input.uv; // Texture���W�w��

	return output;
}
