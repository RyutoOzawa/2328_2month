#pragma once
#include"KEngineFramework.h"
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

	//システム基盤の変数まとめ

	WindowsAPI* windowsAPI = nullptr;
	ReDirectX* directX = nullptr;
	Input* input = nullptr;
	SpriteManager* spriteManager = nullptr;
	ImguiManager* imguiManager = nullptr;
	bool endRequest = false;

	//////////////////////////////////////////////

	//ゲーム内で使用する変数まとめ
	uint32_t magnetTextureN = 0;
	uint32_t magnetTextureS = 0;
	uint32_t groundTexture = 0;
	uint32_t playerTexture = 0;

	Object3d blockObj[10][10][10];

	Player* player = nullptr;

	std::vector<MagnetBlock> magnetBlocks;
	std::vector<MagnetData> magnetDatas;

	Colision* colision = nullptr;

	Camera camera{};

	Map* map_ = nullptr;

};

