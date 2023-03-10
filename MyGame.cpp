#include "MyGame.h"
using namespace DirectX;
#include"SceneFactory.h"

MyGame::MyGame()
{
}

void MyGame::Initialize()
{
	//基底クラスの初期化処理
	KEngineFramework::Initialize();

	//シーンファクトリーの生成とマネージャへセット
	sceneFactory = new SceneFactory();
	GameSceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	//シーンマネージャに最初のシーンをセット
	GameSceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	// 基底クラスの終了処理
	KEngineFramework::Finalize();
}

void MyGame::Update()
{
	//imgui開始処理
	imguiManager->Begin();

	//基底クラスの更新処理
	KEngineFramework::Update();

	//imgui終了
	imguiManager->End();


}

void MyGame::Draw()
{
	directX->BeginDraw();

	//シーン描画処理
	sceneManager->Draw();

//	imguiManager->Draw();
	directX->EndDraw();
}
