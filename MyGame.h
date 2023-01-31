#pragma once
#include"WindowsAPI.h"
#include"DirectX.h"
#include"Input.h"
#include"Texture.h"
#include"SpriteManager.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Camera.h"
#include"ImguiManager.h"
#include"AudioManager.h"
#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"Goal.h"

class MyGame
{
public://メンバ関数
	//初期化
	void Initialize();

	//終了
	void Finalize();

	//毎フレーム更新
	void Update();

	//描画
	void Draw();

	//終了フラグチェック
	bool IsEndReqest() { return endRequest; }

public://メンバ関数

	//システム基盤の変数まとめ

	WindowsAPI* windowsAPI;
	ReDirectX* directX;
	Input* input;
	SpriteManager* spriteManager;
	ImguiManager* imguiManager;
	bool endRequest = false;

	//////////////////////////////////////////////

	//ゲーム内で使用する変数まとめ
	uint32_t magnetTextureN = 0;
	uint32_t magnetTextureS = 0;
	uint32_t groundTexture = 0;
	uint32_t playerTexture = 0;
	uint32_t goalTexture = 0;
	uint32_t clearTexture = 0;

	Object3d blockObj[10][10][10];

	Player* player;

	std::vector<MagnetBlock> magnetBlocks;
	std::vector<MagnetData> magnetDatas;

	Colision* colision;

	Camera camera;

	Map* map_;

	Goal* goal;
	Sprite goalSprite;

};

