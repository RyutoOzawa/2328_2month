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

void Camera::Initialize(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
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
	placePos[0] = { stageSize.x,stageSize.y + (addSizeY  * 2),stageSize.z };

	//�X�e�[�W��X,Z���ꂼ��^��
	float midX = stageSize.x / 2;
	float midZ = stageSize.z / 2;

	//��
	placePos[1] = { midX,stageSize.y + addSizeY,stageSize.z + addSizeZ };
	//��
	placePos[2] = { midX,stageSize.y + addSizeY,0 - addSizeZ };
	//��
	placePos[3] = { 0 - addSizeX ,stageSize.y + addSizeY,midZ };
	//�E
	placePos[4] = { stageSize.x + addSizeX,stageSize.y + addSizeY, midZ };

	isMove = false;

}

void Camera::UpdateEye()
{



	if (isMove == true) {

		MoveEye();

	}
	else {

		if (state == 0) {

			eye = target;
			eye.y = placePos[0].y;
			eye.z = eye.z - 2.5f;

			//placePos[0].x = target.x;
			//placePos[0].y = target.y + addSizeY;
			//placePos[0].z = target.z - 2.5f;

		}
		else if (state == 1) {

			eye.x = placePos[1].x;
			eye.y = placePos[1].y;
			eye.z = placePos[1].z;

		}
		else if (state == 2) {

			eye.x = placePos[2].x;
			eye.y = placePos[2].y;
			eye.z = placePos[2].z;

		}
		else if (state == 3) {

			eye.x = placePos[3].x;
			eye.y = placePos[3].y;
			eye.z = placePos[3].z;

		}
		else if (state == 4) {

			eye.x = placePos[4].x;
			eye.y = placePos[4].y;
			eye.z = placePos[4].z;

		}
	}
}

void Camera::ChangeEye(int place)
{

	isMove = true;
	nextState = place;


	elapsedCount = 0.0f;

	//state == 0 ��x.z�͏�ɕς���Ă��邽�߂����ŏ����X�V
	if (state == 0 || nextState == 0) {
		placePos[0].x = target.x;
		placePos[0].z = target.z - 2.5f;
	}

	start = Vector3(placePos[state].x, placePos[state].y, placePos[state].z);	//�X�^�[�g�n�_
	end = Vector3(placePos[nextState].x, placePos[nextState].y, placePos[nextState].z);	//�G���h�n�_

	//�X�^�[�g�n�_�ƃG���h�n�_�̐^�񒆂𐧌�X��

	float pX;

	if (start.x > end.x) {
		pX = start.x - end.x;
	}
	else {
		pX = end.x - start.x;
	}

	float pZ;

	if (start.z > end.z) {
		pZ = start.z - end.z;
	}
	else {
		pZ = end.z - start.z;
	}

	p = Vector3(pX / 2, addSizeY * 2, pZ / 2);	//����_

	state = place;

}

void Camera::MoveEye()
{

	//�o�ߎ���(elapsedTime [s])�̌v�Z
	elapsedCount++;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.0f;

	//�X�^�[�g�n�_			: start
	// ����_				: p
	//�G���h�n�_			: end
	//�o�ߎ���			: elapsedTime [s]
	//�ړ������̗�(�o�ߎ���/�S�̎���) : timeRate (%)

	timeRate = elapsedTime / maxTime;

	if (timeRate >= 1) {
		isMove = false;
	}

	Vector3 a = lerp(start, p, timeRate);
	Vector3 b = lerp(p, end, timeRate);

	cameraPosition = lerp(a, b, timeRate);

	eye.x = cameraPosition.x;
	eye.y = cameraPosition.y;
	eye.z = cameraPosition.z;

}
