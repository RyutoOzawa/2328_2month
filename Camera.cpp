#include "Camera.h"
#include "Camera.h"
#include"WindowsAPI.h"
using namespace DirectX;

ID3D12Device* Camera::device = nullptr;

void Camera::StaticInitialize(ID3D12Device* dev)
{
	//NULL�`�F�b�N
	assert(dev);
	device = dev;
}

void Camera::Initialize(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up)
{
	//�����������o�Ɉڂ��čs��X�V
	this->eye = eye;
	this->target = target;
	this->up = up;



	HRESULT result;
	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferCamera) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	assert(SUCCEEDED(result));

	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	//��p�̍s���錾
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),					//�㉺��p45�x
		(float)WindowsAPI::winW / WindowsAPI::winH,	//�A�X�y�N�g��i��ʉ���/��ʏc���j
		0.1f, 1000.0f								//�O���A����
	);

	//�r���[�ϊ��s��̌v�Z
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	constMap->view = matView;
	constMap->projection = matProjection;
}

void Camera::InitializeData(Vector3 stageSize)
{
	//�X�e�[�W�̐^��

	stageMid = { stageSize.x / 2,stageSize.y / 2, stageSize.z / 2 };

	stagePos[0] = { stageSize.x,stageSize.y + (addSizeY * 2),stageSize.z };

	//��
	stagePos[1] = { stageMid.x,stageSize.y + addSizeY,stageSize.z + addSizeZ };
	//��
	stagePos[2] = { stageMid.x,stageSize.y + addSizeY,0 - addSizeZ };
	//��
	stagePos[3] = { 0 - addSizeX ,stageSize.y + addSizeY,	stageMid.z };
	//�E
	stagePos[4] = { stageSize.x + addSizeX,stageSize.y + addSizeY, 	stageMid.z };

	isMoveEye = false;

}

void Camera::Update(XMFLOAT3 pPos)
{

	UpdateTarget(pPos);

	UpdateEye();

	UpdateMatrix();

}

void Camera::UpdateEye()
{

	if (isMoveEye == true) {

		MoveEye();

	}
	else {

		if (state_ == 0) {

			eye = target;
			eye.y = stagePos[0].y;
			eye.z = eye.z - 2.5f;

			//placePos[0].x = target.x;
			//placePos[0].y = target.y + addSizeY;
			//placePos[0].z = target.z - 2.5f;

		}
		else if (state_ == 1) {

			eye.x = stagePos[1].x;
			eye.y = stagePos[1].y;
			eye.z = stagePos[1].z;

		}
		else if (state_ == 2) {

			eye.x = stagePos[2].x;
			eye.y = stagePos[2].y;
			eye.z = stagePos[2].z;

		}
		else if (state_ == 3) {

			eye.x = stagePos[3].x;
			eye.y = stagePos[3].y;
			eye.z = stagePos[3].z;

		}
		else if (state_ == 4) {

			eye.x = stagePos[4].x;
			eye.y = stagePos[4].y;
			eye.z = stagePos[4].z;

		}
	}
}

void Camera::UpdateTarget(XMFLOAT3 pPos)
{


	if (state_ == 0) {
		//�J�������_���W�͎��@�ɒǏ]
		target.x = pPos.x;
		target.y = pPos.y;
		target.z = pPos.z;
	}
	else {
		target.x = stageMid.x;
		target.y = 2;
		target.z = stageMid.z;
	}


}

void Camera::ChangeState(int state)
{

	//�x�W�G�ړ����s����true
	isMoveEye = true;
	//�ړ�����Z�b�g
	nextState = state;

	//eye�@�x�W�G���Z�b�g

	elapsedCountEye = 0.0f;

	//state == 0 ��x.z�͏�ɕς���Ă��邽�߂����ŏ����X�V
	if (state_ == 0 || nextState == 0) {
		stagePos[0].x = target.x;
		stagePos[0].z = target.z - 2.5f;
	}

	startEye = Vector3(stagePos[state_].x, stagePos[state_].y, stagePos[state_].z);	//�X�^�[�g�n�_
	endEye = Vector3(stagePos[nextState].x, stagePos[nextState].y, stagePos[nextState].z);	//�G���h�n�_

	//�X�^�[�g�n�_�ƃG���h�n�_�̐^�񒆂𐧌�X��

	float pX;

	if (startEye.x > endEye.x) {
		pX = startEye.x - endEye.x;
	}
	else {
		pX = endEye.x - startEye.x;
	}

	float pZ;

	if (startEye.z > endEye.z) {
		pZ = startEye.z - endEye.z;
	}
	else {
		pZ = endEye.z - startEye.z;
	}

	pEye = Vector3(pX / 2, addSizeY * 2, pZ / 2);	//����_

	this->state_ = state;

}

void Camera::MoveEye()
{

	//�o�ߎ���(elapsedTime [s])�̌v�Z
	elapsedCountEye++;
	float elapsedTime = static_cast<float> (elapsedCountEye) / 60.0f;

	//�X�^�[�g�n�_			: start
	// ����_				: p
	//�G���h�n�_			: end
	//�o�ߎ���			: elapsedTime [s]
	//�ړ������̗�(�o�ߎ���/�S�̎���) : timeRate (%)

	timeRate = elapsedTime / maxTime;

	if (timeRate >= 1) {
		isMoveEye = false;
	}

	Vector3 a = lerp(startEye, pEye, timeRate);
	Vector3 b = lerp(pEye, endEye, timeRate);

	cameraPosition = lerp(a, b, timeRate);

	eye.x = cameraPosition.x;
	eye.y = cameraPosition.y;
	eye.z = cameraPosition.z;

}
