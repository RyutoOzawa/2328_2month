#include "MagnetBlock.h"
//#include"Matrix.h"
#include"Player.h"
//using namespace MathUtility;

MagnetBlock::~MagnetBlock()
{
}

void MagnetBlock::Initialize(MagnetData magnetData)
{
	//デバッグテキストなど、汎用機能のインスタンス取得
	//debugText = DebugText::GetInstance();

	//model = Model::Create();

	//worldTransform.Initialize();

	this->pos = magnetData.pos;

	//引数で受け取った座標を反映
	obj.position = magnetData.pos;

	//オブジェクトデータ初期化
	obj.Initialize();
	obj.model = Model::CreateModel();
	//プレイヤーのデフォルトカラーは白
	//playerTexture = whiteTexture;
	//サイズ調整
	obj.scale = XMFLOAT3(size / 10, size / 10, size / 10);

	isNorth = magnetData.isNorth_;

	for (int i = 0; i < 20; i++) {
		isMagMove[i] = true;

	}

	for (int j = 0; j < 5; j++) {
		contactNum[j] = 100;
	}
}

void MagnetBlock::Update()
{
	obj.position = pos;

	//行列更新
	obj.Update();

	//座標を反映
	//worldTransform.translation_ = pos;
	//worldTransformUpdate(&worldTransform);
	//moveVec = { 0.0f,0.0f,0.0f };z
}

void MagnetBlock::Draw(/*const ViewProjection& viewProjection,*/ const uint32_t& nPoleTexture, const uint32_t& sPoleTexture)
{
	uint32_t magnetTex = 0;
	magnetTex = sPoleTexture;
	if (isNorth) {
		magnetTex = nPoleTexture;
	}
	obj.model->textureIndex = magnetTex;

	obj.Draw();
}

//void MagnetBlock::AddTenPos(XMFLOAT3 pos)
//{
//	this->tentativePos.x += pos.x; 
//	this->tentativePos.y += pos.y;
//	this->tentativePos.z += pos.z;
//}
//
//void MagnetBlock::SubTenPos(XMFLOAT3 pos)
//{
//	this->tentativePos.x -= pos.x;
//	this->tentativePos.y -= pos.y;
//	this->tentativePos.z -= pos.z;
//}

bool MagnetBlock::Colision(XMFLOAT3 pos1, float pos1Size, XMFLOAT3 pos2, float pos2Size)
{
	float pos1X1 = pos1.x - (pos1Size / 2);
	float pos1X2 = pos1.x + (pos1Size / 2);

	float pos1Z1 = pos1.z - (pos1Size / 2);
	float pos1Z2 = pos1.z + (pos1Size / 2);

	float pos2X1 = pos2.x - (pos2Size / 2);
	float pos2X2 = pos2.x + (pos2Size / 2);

	float pos2Z1 = pos2.z - (pos2Size / 2);
	float pos2Z2 = pos2.z + (pos2Size / 2);

	if (pos1X1 < pos2X2 && pos2X1 < pos1X2) {

		if (pos1Z1 < pos2Z2 && pos2Z1 < pos1Z2) {

			return true;

		}
	}

	return false;
}

