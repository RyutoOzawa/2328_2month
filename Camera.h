#pragma once
#include"DirectX.h"
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include "Vector3.h"

class Camera
{
public:
	static ID3D12Device* device;

	static void StaticInitialize(ID3D12Device* dev);

	struct ConstBufferCamera {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	ConstBufferCamera* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matProjection;
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;

public:


	void Initialize(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	void UpdateMatrix();

	DirectX::XMMATRIX GetViewProjection()const { return matView * matProjection; }

	//�}�b�v�̑傫�����󂯎��
	void InitializeData(Vector3 stageSiz);

	//eye�̍X�V
	void UpdateEye();

	//���@�Ǐ],�㉺���E
	void ChangeEye(int place);

	//�x�W�G�Ȑ��ł̈ړ�
	void MoveEye();

private:

	//0���@�Ǐ],1��2��3��4�E
	int state = 0;
	int nextState;

	//�ꏊ���Ƃ̍��W
	Vector3 placePos[5];

	//+��������
	float addSizeX = 10;
	float addSizeY = 10;
	float addSizeZ = 10;


	//��------�x�W�G�Ȑ�-------��

//���Ԍv���ɕK�v�ȃf�[�^
	long long elapsedCount = 0;

	//��ԂŎg���f�[�^
	//start -> end�@�� [s] �Ŋ���������
	Vector3 start;	//�X�^�[�g�n�_
	Vector3 p;	//����_
	Vector3 end;	//�G���h�n�_

	float maxTime = 2.0f;		//�S�̎���[s]
	float timeRate;				//�������Ԃ��i�񂾂�(��)

	Vector3 cameraPosition;
	//�ړ�����
	bool isMove = false;

};

