#pragma once
#include"GameBaseScene.h"

class GameSceneManager
{
private:
	GameSceneManager();
	~GameSceneManager();

public:

	//コピーコンストラクタ無効
	GameSceneManager(const GameSceneManager& obj) = delete;
	//代入演算子を無効
	GameSceneManager& operator=(const GameSceneManager& obj) = delete;

	//インスタンスアクセス専用関数
	static GameSceneManager* GetInstance();

	//更新
	void Update();

	//描画
	void Draw();


	void SetNextScene(GameBaseScene* nextScene) { this->nextScene = nextScene; }

private:
	//現在のシーン
	GameBaseScene* activeScene = nullptr;
	GameBaseScene* nextScene = nullptr;


};

