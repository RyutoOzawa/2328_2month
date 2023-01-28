#pragma once
#include"KEngineFramework.h"

#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"


class MyGame : public KEngineFramework
{
public://メンバ関数

	MyGame();

	//初期化
	void Initialize() override;

	//終了
	void Finalize()override;

	//毎フレーム更新
	void Update()override;

	//描画
	void Draw()override;

public://メンバ関数


	//////////////////////////////////////////////

	ImguiManager* imguiManager = nullptr;	//imgui用

	//ゲーム内で使用する変数まとめ
	uint32_t magnetTextureN = 0;	//N極のテクスチャ
	uint32_t magnetTextureS = 0;	//S極のテクスチャ
	uint32_t groundTexture = 0;		//地面のテクスチャ
	uint32_t playerTexture = 0;		//プレイヤーのテクスチャ

	Object3d blockObj[10][10][10];	//ブロック(マップのオブジェクト)
	
	Player* player = nullptr;		//プレイヤー

	std::vector<MagnetBlock> magnetBlocks;	//磁石オブジェクト
	std::vector<MagnetData> magnetDatas;	//磁石データ格納用

	Colision* colision = nullptr;			//当たり判定管理クラス

	Camera camera{};						//カメラ

	Map* map_ = nullptr;					//マップ情報

};

