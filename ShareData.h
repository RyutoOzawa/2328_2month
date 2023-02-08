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



