#pragma once

#include"Player.h"
#include "Map.h"
#include"MagnetBlock.h"
#include <DirectXMath.h>

using namespace DirectX;

class Colision
{

public:
	/// <summary>
/// コンストクラタ
/// </summary>
	Colision();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Colision();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player, MagnetBlock magnetBlock, int i);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void UpdateDeta(Player* player, MagnetBlock magnetBlock, int i);

	//当たり判定
//マップ参照
	void MapCollision();

	//座標参照
	void PosCollision();

	//磁石の引き寄せ、引き離しの更新処理
	void MagnetsUpdate();

	//磁石同士の引き寄せ、引き離しの更新処理
	void MagToMagUpdate();

	//磁石とプレイヤーの引き寄せ、引き離しの更新処理
	void MagToPlayerUpdate();

	//磁力のON,OFF
	void MagnetPower();

	//座標などの情報を更新
	void InforUpdate();

	//くっついたブロックの移動ベクトルを統一させる
	void StickMag();

	//mainPos のどの面にsubPosにあったっか
	int GetContact(XMFLOAT3 mainPos, XMFLOAT3 subPos);

	//pos2 - pos1　で pos1　から pos2　のベクトルを求める
	XMFLOAT3 GetVec(XMFLOAT3 pos1, XMFLOAT3 pos2);

	//ベクトルを正規化してsizeをかける関数
	XMFLOAT3 ChangeVec(XMFLOAT3 changeVec, float size);

	float lengthVec(XMFLOAT3 vec);

	MagnetBlock GetMagnetBlocks(int i) { return magnetBlocks[i]; }

	std::vector<MagnetBlock> magnetBlocks;

	XMFLOAT3 setPos[20] = {};

public: // メンバ変数

	Player* player = nullptr;

	Map* savemap_ = new Map;

	//std::vector<MagnetData> magnetDatas;

	XMFLOAT3 vecMagToMag;

	//自機当たっているか
	XMFLOAT2 ColX = { 0,0 };
	XMFLOAT2 ColY = { 0,0 };
	XMFLOAT2 ColZ = { 0,0 };

	//くっついているブロックの番号
	bool stickBlockMass[20] = { 0,0,0,0 };

	//くっついているブロックの中でどれが1番プレイヤーに近いか
	int mostNearPlayerBlock = 0;
	bool mostNear;

	//周りの4面のみ
	int mostNearPlayerBlockFour = 0;
	bool mostNearFour;

	int a;

	//ブロックの最大数
	static const int blockSizeMax = 20;

	//nブロック 座標

	XMFLOAT3 bPos[blockSizeMax];

	float bPosX1[blockSizeMax] = {};
	float bPosX2[blockSizeMax] = {};

	float bPosZ1[blockSizeMax] = {};
	float bPosZ2[blockSizeMax] = {};

	//ブロックの当たり判定の大きさ
	float bSize = 0.99f; // (2 * 0.99)


	//XMFLOAT3 setPos[blockSizeMax] = {};

	//XMFLOAT3 bMoveVec;


	XMFLOAT3 bMoveVec[blockSizeMax] = {};

	//XMFLOAT3 moveVec2 = { };


	//--------自機----------

	XMFLOAT3 pPos;
	float pSize;
	int pState;

	float pPosX1;
	float pPosX2;

	float pPosZ1;
	float pPosZ2;

	XMFLOAT3 pMoveVec;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

};

