#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include"Input.h"
#include<DirectXTex.h>
//#include"WindowsAPI.h"
//#include"DirectX.h"
#include"Object3d.h"
#include"Util.h"
#include"Texture.h"
#include"GpPipeline.h"
#include"Gridline.h"
#include<string>
#include"SpriteManager.h"
#include"Sprite.h"
#include"Material.h"
using namespace Microsoft::WRL;
#include"Matrix4.h"
#include"Map.h"
#include"Player.h"
#include"MagnetBlock.h"
#include"Camera.h"

//パイプラインステートとルートシグネチャのセット
struct PipelineSet {
	//パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
};


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 基盤システム初期化
	//windowsAPI初期化処理
	WindowsAPI* windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX初期化処理
	ReDirectX* directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	HRESULT result{};

	//キーボード初期化処理
	Input* input = new Input();
	input->Initialize(windowsAPI);

	//テクスチャマネージャーの初期化
	Texture::Initialize(directX->GetDevice());

	SpriteManager* spriteManager = nullptr;
	//スプライト共通部の初期化
	spriteManager = new SpriteManager;
	spriteManager->Initialize(directX, WindowsAPI::winW, WindowsAPI::winH);

	//3Dオブジェクトの初期化
	Object3d::StaticInitialize(directX);

	//カメラクラス初期化
	Camera::StaticInitialize(directX->GetDevice());

#pragma endregion 基盤システム初期化

#pragma region 描画初期化処理

	///-------------------------------///
	/// 　ゲームループで使用する変数の宣言   ///
	///-------------------------------///

	//スプライト一枚の初期化
	/*Sprite* sprite = new Sprite();
	sprite->Initialize(spriteManager,marioGraph);

	Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spriteManager,reimuGraph);*/
	//sprite2->SetTextureNum(1);

	//画像読み込み
	uint32_t magnetTextureN = Texture::LoadTexture(L"Resources/red1x1.png");
	uint32_t magnetTextureS = Texture::LoadTexture(L"Resources/blue1x1.png");
	uint32_t groundTexture = Texture::LoadTexture(L"Resources/ground.png");
	uint32_t playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");

		//マップの座標
	Object3d blockObj[10][10][10];

	Player* player = nullptr;

	std::vector<MagnetBlock> magnetBlocks;



	//ビュー行列、射影行列に必要な変数宣言
	XMFLOAT3 eye(5, 25, 6);	//視点座標
	XMFLOAT3 target(5, 0, 6);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル

	/*eye = XMFLOAT3(0.0f, 0.0f, 0.0f);
	target = XMFLOAT3(0, 0, 10);
	up = XMFLOAT3(0, 1, 0);*/

	Camera camera;
	camera.Initialize(eye,target,up);

	Model* skydome;
	skydome = Model::CreateModel("skydome");
	Object3d skydomeObj;
	skydomeObj.Initialize();
	skydomeObj.SetModel(skydome);
	skydomeObj.position = XMFLOAT3(0, 0.0f, 10.0f);

	///-------------------------------///
	/// 　変数の宣言ここまで			  ///
	///-------------------------------///

//------------------------------------------------------------------------

	///-------------------------------///
	/// 　ゲームループの初期化処理         ///
	///-------------------------------///


	//透視東映返還行列の計算
	//matProjection = XMMatrixPerspectiveFovLH(
	//	XMConvertToRadians(45.0f),					//上下画角45度
	//	(float)WindowsAPI::winW / WindowsAPI::winH,	//アスペクト比（画面横幅/画面縦幅）
	//	0.1f, 1000.0f								//前橋、奥橋
	//);

	////ビュー変換行列の計算
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));


	//Map読み込み
	Map* map_ = new Map;
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

	//camera.target = blockObj[0][0][0].position;

	//プレイヤー初期化
	player = new Player();
	player->Initialize(playerTexture, magnetTextureN, magnetTextureS, input);
	player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);

	///-------------------------------///
	/// 　初期化処理ここまで	　        ///
	///-------------------------------///


#pragma endregion 描画初期化処理
	// ゲームループ
	while (true) {

#pragma region 基盤システム更新
		//windowsのメッセージ処理
		if (windowsAPI->ProcessMessage()) {
			//ループを抜ける
			break;
		}
		input->Update();
#pragma endregion 基盤システム更新

#pragma region シーン更新処理

		player->Update();

		skydomeObj.Update();

		//カメラ座標は自機に追従
		camera.target.x = player->GetPosition().x;
		camera.target.y = player->GetPosition().y;
		camera.target.z= player->GetPosition().z;
		camera.eye = camera.target;
		camera.eye.y += 20.0f;
		camera.eye.z -= 2.5f;

		camera.UpdateMatrix();

#pragma endregion シーン更新処理

		//描画前処理
		directX->BeginDraw();
#pragma region シーン描画処理

		//3Dオブジェクト描画処理
		Object3d::BeginDraw(camera);
		//object1.Draw();

		player->Draw();

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

		//skydomeObj.Draw();

		//スプライト描画処理
		spriteManager->beginDraw();
		//sprite->Draw();
		//sprite2->Draw();



#pragma endregion シーン描画処理
		// ４．描画コマンドここまで
		directX->EndDraw();
		// DirectX毎フレーム処理 ここまで
	}
#pragma region シーン終了処理
	//WindowsAPI終了処理
	windowsAPI->Finalize();

	//基盤システムを初期化した順番に解放。その後ゲームループで使用した物を解放
	delete windowsAPI;
	delete input;
	delete directX;
	delete spriteManager;

	//ここからゲームループで使用したもの
	//delete sprite;
	//delete skyDome;
	delete map_;
	delete player;


#pragma endregion シーン終了処理
	return 0;
}
