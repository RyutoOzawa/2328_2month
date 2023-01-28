#include "MyGame.h"
using namespace DirectX;



MyGame::MyGame()
{
}

void MyGame::Initialize()
{

	//基底クラスの初期化処理
	KEngineFramework::Initialize();

	//--------------ゲーム内変数初期化--------------//

	imguiManager = new ImguiManager();
	imguiManager->Initialize(windowsAPI,directX);

	//テクスチャデータ初期化
	magnetTextureN = Texture::LoadTexture(L"Resources/red1x1.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/blue1x1.png");
	groundTexture = Texture::LoadTexture(L"Resources/ground.png");
	playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");

	//カメラ初期化
	XMFLOAT3 eye(5, 25, 6);	//視点座標
	XMFLOAT3 target(5, 0, 6);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル

	camera.Initialize(eye, target, up);

	//マップ読み込み
	map_ = new Map;
	map_->Loding("map/map1.csv");

	//マップの座標の初期化
	for (int i = 0; i < blockY; i++)
	{
		for (int j = 0; j < blockZ; j++)
		{
			for (int k = 0; k < blockX; k++)
			{
				blockObj[i][j][k].Initialize();
				blockObj[i][j][k].model = Model::CreateModel();
				blockObj[i][j][k].model->textureIndex = groundTexture;
				blockObj[i][j][k].position.x = k * blockSize * blockScale;
				blockObj[i][j][k].position.y = i * blockSize * blockScale;
				blockObj[i][j][k].position.z = j * blockSize * blockScale;
				blockObj[i][j][k].scale = { blockScale,blockScale,blockScale };
				blockObj[i][j][k].Update();
			}
		}
	}

	//プレイヤー初期化
	player = new Player();
	player->Initialize(playerTexture, magnetTextureN, magnetTextureS, input);
	player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);

	//磁石データ初期化
	MagnetData nBlockPos{ XMFLOAT3(3,2,2),true };
	MagnetData sBlockPos{ XMFLOAT3(6,2,2), false };
	MagnetData n2BlockPos{ XMFLOAT3(3, 2, 5), true };
	MagnetData s2BlockPos{ XMFLOAT3(6, 2, 5), false };

	//磁石データを配列に差し込む
	magnetDatas.push_back(nBlockPos);
	magnetDatas.push_back(sBlockPos);
	magnetDatas.push_back(n2BlockPos);
	magnetDatas.push_back(s2BlockPos);

	//磁石の初期化と生成
	for (int i = 0; i < magnetDatas.size(); i++) {
		MagnetBlock newBlock{};
		newBlock.Initialize(magnetDatas[i]);
		//ゲームで使うようの配列に格納
		magnetBlocks.push_back(newBlock);
		magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	}

	//当たり判定初期化
	colision = new Colision();
	for (int i = 0; i < magnetDatas.size(); i++) {
		colision->Initialize(player, magnetBlocks[i], i);
	}

}

void MyGame::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//ここからゲームループで使用したもの
	imguiManager->Finalize();
	delete imguiManager;
	//delete sprite;
	//delete skyDome;
	delete map_;
	delete player;

	//-------------ここまでにループ内で使用したものの後処理------------//

	//基底クラスの終了処理
	KEngineFramework::Finalize();

}

void MyGame::Update()
{
	//基底クラスの更新処理
	KEngineFramework::Update();

	//----------------------ゲーム内ループはここから---------------------//

	//imgui受付開始処理
	imguiManager->Begin();


	//磁力計算
	for (int i = 0; i < magnetDatas.size(); i++) {
		colision->UpdateDeta(player, magnetBlocks[i], i);
	}

	colision->Update();

	//座標の更新
	for (int i = 0; i < magnetBlocks.size(); i++) {

		magnetBlocks[i] = colision->magnetBlocks[i];

		magnetBlocks[i].Update();

	}

	player->Update();

	//カメラ座標は自機に追従
	camera.target.x = player->GetPosition().x;
	camera.target.y = player->GetPosition().y;
	camera.target.z = player->GetPosition().z;
	camera.eye = camera.target;
	camera.eye.y += 20.0f;
	camera.eye.z -= 2.5f;

	camera.UpdateMatrix();

	//----------------------ゲーム内ループはここまで---------------------//

	//imgui受付終了
	imguiManager->End();

}

void MyGame::Draw()
{
	//描画前処理
	directX->BeginDraw();

	//-------背景スプライト描画処理-------//
	spriteManager->beginDraw();


	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(camera);

	//自機描画
	player->Draw();

	//磁石描画
	for (int i = 0; i < magnetBlocks.size(); i++) {
		magnetBlocks[i].Draw(magnetTextureN, magnetTextureS);
	}

	//マップの描画
	for (int i = 0; i < blockY; i++)
	{
		for (int j = 0; j < blockZ; j++)
		{
			for (int k = 0; k < blockX; k++)
			{
				if (map_->map[i][j][k] == 1)
				{
					blockObj[i][j][k].Draw();
				}
			}
		}
	}


	//-------前景スプライト描画処理-------//
	spriteManager->beginDraw();


	//デバッグ描画
	imguiManager->Draw();

	//描画後処理
	directX->EndDraw();
}
