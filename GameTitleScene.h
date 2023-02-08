#pragma once
#include"GameBaseScene.h"

#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include"Util.h"
#include "AudioManager.h"


class GameTitleScene :public GameBaseScene
{
public:
	//初期化
	void Initialize() override;

	//終了
	void Finalize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;


public://メンバ変数

	ImguiManager* imguiManager = nullptr;	//imgui用
	Input* input = nullptr;

	//ゲーム内で使用する変数まとめ

	uint32_t titleTexture = 0;	//背景画像(現在はダミー
	Sprite* titleSprite = nullptr;

	uint32_t uiButtonATexture = 0;
	Sprite* uiButtonASprite = nullptr;

	uint32_t uiStageSelectTexture = 0;
	Sprite* uiStageSelectSprite = nullptr;

	uint32_t uiStageNumberTexture[11] = {};
	Sprite* uiStageNumberSprite[11] = {};

	uint32_t sceneChangeTexture[2]{};
	Sprite* sceneChangeSprite[2]{};

	uint32_t  titleBackTexture[4]{};
	Sprite* titleBackSprite[4]{};

	AudioManager *titleBGM;
	float titleBGMVolume = 0.1f;
	AudioManager *decisionSE;
	float decisionSEVolume = 1.0f;
	AudioManager *serectSE;
	float serectSEVolume = 1.0f;

	int phase = 0;
	 
	float sinAngle = 0.0f;

};

