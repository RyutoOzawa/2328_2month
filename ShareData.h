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
};


