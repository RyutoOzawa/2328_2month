#pragma once
#include"GameBaseScene.h"
#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include"Goal.h"
#include "AudioManager.h"

enum MenuIndex {
	Reset,
	StageSelect_MENU,
	Title,
};

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

	//ステージリセット
	void StageInitialize(int stageNumber);

	//タイトルに戻る
	void GoTitle();

	//ステージセレクトに戻る
	void GoStageSelect();

public://メンバ変数

	//ImguiManager* imguiManager = nullptr;	//imgui用
	Input* input = nullptr;

		//ゲーム内で使用する変数まとめ
	uint32_t magnetTextureN = 0;	//N極のテクスチャ
	uint32_t magnetTextureS = 0;	//S極のテクスチャ
	uint32_t groundTexture = 0;		//地面のテクスチャ
	uint32_t groundTextures[4]{};	//地面
	uint32_t playerTexture = 0;		//プレイヤーのテクスチャ
	uint32_t playerTextureN = 0;		//プレイヤーNのテクスチャ
	uint32_t playerTextureS = 0;		//プレイヤーSのテクスチャ

	uint32_t clearTexture = 0;		//クリア画面
	uint32_t clearNextTexture = 0;		//クリア画面
	uint32_t clearStageSerectTexture = 0;		//クリア画面

	uint32_t goalTexture = 0;		//ゴールのテクスチャ
	uint32_t menuTexture = 0;		//メニューのテクスチャ
	uint32_t menuResetTexture = 0;		//メニューのResetのテクスチャ
	uint32_t menuTitleTexture = 0;		//メニューのTitleのテクスチャ
	uint32_t menuStageSerectTexture = 0;		//メニューのStageSerectのテクスチャ

	uint32_t  backGroundTexture[4]{};//背景
	Sprite* backGroundSprite[4]{};

	Sprite* menuSprite = nullptr;
	Sprite* menuResetSprite = nullptr;
	Sprite* menuTitleSprite = nullptr;
	Sprite* menuStageSerectSprite = nullptr;

	//クリア画面
	Sprite* clearSprite = nullptr;
	Sprite *clearNextSprite = nullptr;
	Sprite *clearStageSerectSprite = nullptr;


	uint32_t selectBoxTexture = 0;
	Sprite* selectBoxSprite = nullptr;

	uint32_t playUITexture =0;
	Sprite* playUISprite = nullptr;

	Sprite* sceneChangeSprite[2]{};


	DirectX::XMFLOAT2 boxPos{};
	DirectX::XMFLOAT2 selectBoxPos[3]{};

	Object3d blockObj[20][20][20];	//ブロック(マップのオブジェクト)
	Model* blockModel = nullptr;

	DirectX::XMFLOAT3 rota;

	Player* player = nullptr;		//プレイヤー

	std::vector<MagnetBlock> magnetBlocks;	//磁石オブジェクト
	std::vector<MagnetData> magnetDatas;	//磁石データ格納用

	Colision* colision = nullptr;			//当たり判定管理クラス

	Camera camera{};						//カメラ

	Map* map_ = nullptr;					//マップ情報

	Goal* goal;

	//ステージファイル名
	std::string stageStr;

	//ステージ大きさ
	Vector3 stageSize = {};

	//音
	AudioManager* playBGM;
	float playBGMVolume = 0.1f;
	AudioManager* decisionSE;
	float decisionSEVolume = 1.0f;
	AudioManager* serectSE;
	float serectSEVolume = 1.0f;

	//メニューを開いているかどうか
	bool isMenu = false;
	int selectMenuNumber = 0;
	int clearMenuNumber = 0;

	int cameraState = 0;

	float sinAngle = 0.0f;

};

