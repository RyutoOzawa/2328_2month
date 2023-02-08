#pragma once
enum StageIndex {
	Tutoattract,	//0
	Whichload,		//1
	Dontpanic,		//2
	Switching,		//3
	Down,			//4
	Order,			//5
	Tutorepulsion,	//6
	Direction,		//7
	Needmagnet,		//8
	Jam,			//9
	Mislead,		//10
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


