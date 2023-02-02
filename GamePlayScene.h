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

	//制御店の集合(vectorコンテナ)、補間する区間の添え字、時間経過率
	Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

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
	uint32_t menuTexture = 0;		//メニューのテクスチャ

	uint32_t backGroundTexture = 0;	//背景画像(現在はダミー
	Sprite* backGroundSprite = nullptr;
	Sprite* menuSprite = nullptr;

	uint32_t selectBoxTexture = 0;
	Sprite* selectBoxSprite = nullptr;

	uint32_t playUITexture =0;
	Sprite* playUISprite = nullptr;

	DirectX::XMFLOAT2 boxPos{};
	DirectX::XMFLOAT2 selectBoxPos[3]{};

	Object3d blockObj[20][20][20];	//ブロック(マップのオブジェクト)

	Player* player = nullptr;		//プレイヤー

	std::vector<MagnetBlock> magnetBlocks;	//磁石オブジェクト
	std::vector<MagnetData> magnetDatas;	//磁石データ格納用

	Colision* colision = nullptr;			//当たり判定管理クラス

	Camera camera{};						//カメラ

	Map* map_ = nullptr;					//マップ情報

	Goal* goal;
	Sprite goalSprite;

	//ステージファイル名
	std::string stageStr;

	//ステージ大きさ
	Vector3 stageSize = {};

	//メニューを開いているかどうか
	bool isMenu = false;
	int selectMenuNumber = 0;
	int clearMenuNumber = 0;

	//↓------ベジエ曲線-------↓

	//時間計測に必要なデータ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//補間で使うデータ
	//start -> end　を [s] で完了させる
	Vector3 start;	//スタート地点
	Vector3 p ;	//制御点
	Vector3 end ;	//エンド地点

	std::vector<Vector3> points{};

	//p1からスタートする
	size_t startIndex = 1;

	float maxTime = 5.0f;		//全体時間[s]
	float timeRate;				//何％時間が進んだか(率)

	Vector3 position;

};

