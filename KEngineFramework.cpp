#include "KEngineFramework.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
}

void KEngineFramework::Finalize()
{
}

void KEngineFramework::Update()
{
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
