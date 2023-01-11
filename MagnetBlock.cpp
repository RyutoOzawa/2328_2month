#include "MagnetBlock.h"
using namespace DirectX;

void MagnetBlock::Initialize(const XMFLOAT3& pos, bool isNorth_)
{
	//���f�������A������
	obj->model->CreateModel();
	obj->Initialize();
	this->pos = pos;

	//�����Ŏ󂯎�������W�𔽉f
	obj->position = pos;
	obj->scale = XMFLOAT3(0.99f, 0.99f, 0.99f);
}

void MagnetBlock::Update(const XMFLOAT3& playerPos, int playerState, float moveDistance,XMMATRIX& matView,XMMATRIX& matProjection)
{
	if (move == 1) {
		//���@�̏�Ԃ����΂Ȃ�����񂹓��̏������s��
		if (playerState != UnMagnet) {
			bool isPlayerNorth = false;
			if (playerState == NorthPole) {
				isPlayerNorth = true;
			}
			else if (playerState == SouthPole) {
				isPlayerNorth = false;
			}
			bool isPull;
			if (isPlayerNorth == isNorth) {

				isPull = false;
			}
			else {
				isPull = true;
			}

	

			//���@���W���Q�Ƃ��A���@�Ǝ��΂̋������v�Z
			XMFLOAT3 vecPlayerToblock;
			vecPlayerToblock.x = playerPos.x - pos.x;
			vecPlayerToblock.y = playerPos.y - pos.y;
			vecPlayerToblock.z = playerPos.z - pos.z;
			//�x�N�g���̒����͈ړ��J�n�����ȉ��Ȃ玩�@�A���΂̎��͂��g���Ĉ����񂹓��̏���
			float vecLength = 0;
			XMStoreFloat(&vecLength, XMVector3Length(XMLoadFloat3(&vecPlayerToblock)));
			if (isPull) {

				if (vecLength <= moveDistance) {
					/*Vector3 moveVec;*/
					XMStoreFloat3(&moveVec, XMVector3Normalize(XMLoadFloat3(&vecPlayerToblock)));
					moveVec.x *= moveSpd;
					moveVec.y *= moveSpd;
					moveVec.z *= moveSpd;
					pos.x += moveVec.x;
					pos.y += moveVec.y;
					pos.z += moveVec.z;
				}
			}
			else {

				if (vecLength <= moveDistance) {
					/*Vector3 moveVec;*/
					XMStoreFloat3(&moveVec, XMVector3Normalize(XMLoadFloat3(&vecPlayerToblock)));
					moveVec.x *= moveSpd;
					moveVec.y *= moveSpd;
					moveVec.z *= moveSpd;
					pos.x -= moveVec.x;
					pos.y -= moveVec.y;
					pos.z -= moveVec.z;
				}
			}
		}

	}

	//���W�𔽉f
	obj->position = pos;
	obj->Update(matView,matProjection);
}

void MagnetBlock::Draw(const uint32_t& nPoleTexuture, const uint32_t& sPoleTexture)
{
	uint32_t magTex = 0;
	magTex = sPoleTexture;
	if (isNorth) {
		magTex = nPoleTexuture;
	}
	obj->model->textureIndex = magTex;
	obj->Draw();
}
