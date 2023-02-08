#pragma once
#include"DirectX.h"
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include "Vector3.h"

using namespace DirectX;

class Camera
{
public:
	static ID3D12Device* device;

	static void StaticInitialize(ID3D12Device* dev);

	struct ConstBufferCamera {
		XMMATRIX view;
		XMMATRIX projection;
	};

	ConstBufferCamera* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	XMMATRIX matView;
	XMMATRIX matProjection;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

public:


	void Initialize(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up);

	void UpdateMatrix();

	XMMATRIX GetViewProjection()const { return matView * matProjection; }

	//�}�b�v�̑傫�����󂯎��
	void InitializeData(Vector3 stageSiz);

	void Update(XMFLOAT3 pPos);

	//eye�̍X�V
	void UpdateEye();

	//target�̍X�V
	void UpdateTarget(XMFLOAT3 pPos);

	//���@�Ǐ],�㉺���E
	void ChangeState(int state);

	//�x�W�G�Ȑ��ł̈ړ�
	void MoveEye();

	//�J�����ƒ��S�_�̃x�N�g��

	XMFLOAT3 GetCameraVec() { return cameraVec; }

	void UpdateCameraVec();

	bool GetisMoveEye() { return isMoveEye; }

private:

	//0���@�Ǐ],1��2��3��4�E
	int state_ = 0;
	int nextState;

	//�ꏊ���Ƃ̍��W
	Vector3 stagePos[5];

	//�X�e�[�W�̒��S
	Vector3 stageMid;

	//+��������
	float addSizeX = 10;
	float addSizeY = 10;
	float addSizeZ = 10;

	//�J�����ƒ��S�_�̃x�N�g��
	XMFLOAT3 cameraVec;

	//��------�x�W�G�Ȑ�-------��


	float maxTime = 2.0f;		//�S�̎���[s]
	float timeRate;				//�������Ԃ��i�񂾂�(��)

	//-----eye-----

	//���Ԍv���ɕK�v�ȃf�[�^
	long long elapsedCountEye = 0;

	//��ԂŎg���f�[�^
	//start -> end�@�� [s] �Ŋ���������
	Vector3 startEye;	//�X�^�[�g�n�_
	Vector3 pEye;	//����_
	Vector3 endEye;	//�G���h�n�_

	Vector3 cameraPosition;

	//�ړ�����
	bool isMoveEye = false;

};

