#pragma once
#include"Util.h"

enum StageIndex {
	Sample1,
	Sample2,
	Sample3,
	Sample4,
	tutorial1,
	StageIndexCount,
};

enum TitlePhaseIndex {
	WaitInputSpaceKey,//スペースキーの入力待ち
	StageSelect,
};

class ShareData
{
public:
	//選択されたステージ番号
	static int stageNumber;

	//タイトルシーンの進行状態
	static int titlePhase;

	//シーンチェンジ用の変数
	static EaseingData sceneChangeEase;
	static DirectX::XMFLOAT2 easePos[2][2];
	static DirectX::XMFLOAT2 nextPos[2];
	static bool isActiveSceneChange;
	static bool isBeforeSceneClosed;

	static void OpenSceneChange();
	static void CloseSceneChange();
};



