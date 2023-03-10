#include "KEngineFramework.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
	//windowsAPI初期化処理
	windowsAPI = WindowsAPI::GetInstance();
	windowsAPI->Initialize();

	// DirectX初期化処理
	directX = ReDirectX::GetInstance();
	directX->Initialize(windowsAPI);

	//キーボード初期化処理
	input =  Input::GetInstance();
	input->Initialize();

	//テクスチャマネージャーの初期化
	Texture::Initialize(directX->GetDevice());

	//スプライト共通部の初期化
	spriteManager = SpriteManager::GetInstance();
	spriteManager->Initialize(directX, WindowsAPI::winW, WindowsAPI::winH);

	//3Dオブジェクトの初期化
	Object3d::StaticInitialize(directX);

	//カメラクラス初期化
	Camera::StaticInitialize(directX->GetDevice());

	//オーディオ初期化
	AudioManager::StaticInitialize();

	//デバッグテキスト(imgui初期化)
	imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	//シーンマネージャの生成
	sceneManager = GameSceneManager::GetInstance();
}

void KEngineFramework::Finalize()
{
	delete sceneFactory;
	imguiManager->Finalize();

	windowsAPI->Finalize();
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

	//シーンマネージャの更新処理
	sceneManager->Update();
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
