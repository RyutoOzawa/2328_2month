﻿#include <DirectXMath.h>
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


#pragma endregion 基盤システム初期化

#pragma region 描画初期化処理

	//画像読み込み
	uint32_t magnetTextureN = Texture::LoadTexture(L"Resources/red1×1.jpg");
	uint32_t magnetTextureS = Texture::LoadTexture(L"Resources/blue1×1.png");

	//スプライト一枚の初期化
	/*Sprite* sprite = new Sprite();
	sprite->Initialize(spriteManager,marioGraph);

	Sprite* sprite2 = new Sprite();
	sprite2->Initialize(spriteManager,reimuGraph);*/
	//sprite2->SetTextureNum(1);

	Model* skyDome;
	skyDome = Model::CreateModel("skydome");

	Object3d object1;
	object1.Initialize();
	object1.SetModel(skyDome);
	//object1.scale = XMFLOAT3(0.2f, 0.2f, 0.2f);
	object1.position = XMFLOAT3(0, 0, 50.0f);

	//ランダムな数値を取得
	float randValue = Random(-100, 100);

	//ワールド変換行列
//	XMMATRIX matWorld0;
	//XMMATRIX matWorld1;

	const size_t kObjCount = 50;
	Object3d obj[kObjCount];

	Object3d object;

	//ビュー行列、射影行列に必要な変数宣言
	XMMATRIX matProjection;
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, 0);	//視点座標
	XMFLOAT3 target(0, 0, 10);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル

	//透視東映返還行列の計算
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),					//上下画角45度
		(float)WindowsAPI::winW / WindowsAPI::winH,	//アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f								//前橋、奥橋
	);

	//ビュー変換行列の計算
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

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

		if (input->IsPress(DIK_A)) {
			object1.rotation.y += 0.1f;
		}
		else if (input->IsPress(DIK_D)) {
			object1.rotation.y -= 0.1f;
		}
		if (input->IsPress(DIK_W)) {
			object1.rotation.z += 0.1f;
		}
		else if (input->IsPress(DIK_S)) {
			object1.rotation.z -= 0.1f;
		}

		//object1.scale = { 50,50,50 };

		object1.Update(matView, matProjection);

#pragma endregion シーン更新処理

		//描画前処理
		directX->BeginDraw();
#pragma region シーン描画処理

		//3Dオブジェクト描画処理
		Object3d::BeginDraw();
		object1.Draw();

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
	delete skyDome;

#pragma endregion シーン終了処理
	return 0;
}
