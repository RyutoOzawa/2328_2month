#pragma once
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
};


