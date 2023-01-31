#pragma once
#include"GameBaseScene.h"
#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"



class GamePlayScene:public GameBaseScene
{
public:
	//初期化
	void Initialize()override;

	//終了
	void Finalize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	void SetStage(int stageNumber);


public://メンバ変数

	ImguiManager* imguiManager = nullptr;	//imgui用
	Input* input = nullptr;

		//ゲーム内で使用する変数まとめ
	uint32_t magnetTextureN = 0;	//N極のテクスチャ
	uint32_t magnetTextureS = 0;	//S極のテクスチャ
	uint32_t groundTexture = 0;		//地面のテクスチャ
	uint32_t playerTexture = 0;		//プレイヤーのテクスチャ
	uint32_t clearTexture = 0;		//クリア画面
	uint32_t goalTexture = 0;		//ゴールのテクスチャ

	uint32_t backGroundTexture = 0;	//背景画像(現在はダミー
	Sprite* backGroundSprite = nullptr;

	Object3d blockObj[blockY][blockX][blockZ];	//ブロック(マップのオブジェクト)

	Player* player = nullptr;		//プレイヤー

	std::vector<MagnetBlock> magnetBlocks;	//磁石オブジェクト
	std::vector<MagnetData> magnetDatas;	//磁石データ格納用

	Colision* colision = nullptr;			//当たり判定管理クラス

	Camera camera{};						//カメラ

	Map* map_ = nullptr;					//マップ情報

	Goal* goal;
	Sprite goalSprite;

	std::string stageStr;


};

