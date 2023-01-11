#include "MagnetBlock.h"
using namespace DirectX;

void MagnetBlock::Initialize(const XMFLOAT3& pos, bool isNorth_)
{
	//モデル生成、初期化
	obj->model->CreateModel();
	obj->Initialize();
	this->pos = pos;

	//引数で受け取った座標を反映
	obj->position = pos;
	obj->scale = XMFLOAT3(0.99f, 0.99f, 0.99f);
}

void MagnetBlock::Update(const XMFLOAT3& playerPos, int playerState, float moveDistance,XMMATRIX& matView,XMMATRIX& matProjection)
{
	if (move == 1) {
		//自機の状態が磁石なら引き寄せ等の処理を行う
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

	

			//自機座標を参照し、自機と磁石の距離を計算
			XMFLOAT3 vecPlayerToblock;
			vecPlayerToblock.x = playerPos.x - pos.x;
			vecPlayerToblock.y = playerPos.y - pos.y;
			vecPlayerToblock.z = playerPos.z - pos.z;
			//ベクトルの長さは移動開始距離以下なら自機、磁石の磁力を使って引き寄せ等の処理
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

	//座標を反映
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
