#include "KEngineFramework.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
	HRESULT result;
	//windowsAPI初期化処理
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX初期化処理
	directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	//キーボード初期化処理
	input = new Input();
	input->Initialize(windowsAPI);

	//テクスチャマネージャーの初期化
	Texture::Initialize(directX->GetDevice());

	//スプライト共通部の初期化
	spriteManager = new SpriteManager;
	spriteManager->Initialize(directX, WindowsAPI::winW, WindowsAPI::winH);

	//3Dオブジェクトの初期化
	Object3d::StaticInitialize(directX);

	//カメラクラス初期化
	Camera::StaticInitialize(directX->GetDevice());


	//オーディオ初期化
	AudioManager::StaticInitialize();

}

void KEngineFramework::Finalize()
{
	windowsAPI->Finalize();

	delete windowsAPI;
	delete input;
	delete directX;
	delete spriteManager;


}

void KEngineFramework::Update()
{
	//windowsのメッセージ処理
	if (windowsAPI->ProcessMessage()) {
		//ループを抜ける
		endRequest = true;
	}

	//入力系更新
	input->Update();
}

void KEngineFramework::Run()
{
	//初期化
	Initialize();

	//ゲームループ
	while (true)
	{
		//毎フレーム更新
		Update();
		if (IsEndRequest()) {
			break;
		}
		//描画
		Draw();
	}

	//終了処理
	Finalize();
}
