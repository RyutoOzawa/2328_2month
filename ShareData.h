#pragma once
#include"Util.h"

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
	WaitInputSpaceKey,//�X�y�[�X�L�[�̓��͑҂�
	StageSelect,
};

class ShareData
{
public:
	//�I�����ꂽ�X�e�[�W�ԍ�
	static int stageNumber;

	//�^�C�g���V�[���̐i�s���
	static int titlePhase;

	//�V�[���`�F���W�p�̕ϐ�
	static EaseingData sceneChangeEase;
	static DirectX::XMFLOAT2 easePos[2][2];
	static DirectX::XMFLOAT2 nextPos[2];
	static bool isActiveSceneChange;
	static bool isBeforeSceneClosed;

	static void OpenSceneChange();
	static void CloseSceneChange();
};



