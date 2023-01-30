#include "GameSceneManager.h"
#include"GameTitleScene.h"

GameSceneManager::GameSceneManager()
{
	//最初はタイトルシーン
	GameBaseScene* firstScene = new GameTitleScene();
	SetNextScene(firstScene);
}

GameSceneManager::~GameSceneManager()
{
	//現在のシーンの終了と開放
	activeScene->Finalize();
	delete activeScene;
}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager instance;
	return &instance;
}

void GameSceneManager::Update()
{
	
	//次のシーンが予約されているなら
	if (nextScene) {
		//旧シーン終了
		if (activeScene) {
			activeScene->Finalize();
			delete activeScene;
		}

		//シーン切り替え
		activeScene = nextScene;
		nextScene = nullptr;

		//シーン側からシーン切り替えを依頼できるように、シーンマネージャをセットする
		activeScene->SetSceneManager(this);

		//次シーン初期化
		activeScene->Initialize();

	}
	//実行シーンの更新
	activeScene->Update();
}

void GameSceneManager::Draw()
{
	activeScene->Draw();
}
