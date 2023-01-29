#include "MyGame.h"
using namespace DirectX;



MyGame::MyGame()
{
}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	KEngineFramework::Initialize();


	//ゲームシーンの生成と初期化
	gameScene = new GamePlayScene();
	gameScene->Initialize();
	
}

void MyGame::Finalize()
{
	//シーンの終了と開放
	gameScene->Finalize();
	delete gameScene;

	// 基底クラスの終了処理
	KEngineFramework::Finalize();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	KEngineFramework::Update();

	//imgui開始処理
	imguiManager->Begin();

	//シーン更新処理
	gameScene->Update();

	//imgui終了
	imguiManager->End();


}

void MyGame::Draw()
{
	directX->BeginDraw();

	gameScene->Draw();

	imguiManager->Draw();
	directX->EndDraw();
}
