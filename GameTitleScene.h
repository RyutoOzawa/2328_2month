#pragma once
#include"GameBaseScene.h"

#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"
#include"Util.h"
#include"AudioManager.h"

enum StageSelectPosIndex {
	LeftLeft,
	Left,
	Center,
	Right,
	RightRight,
	StageSelectPosCount,
};

class GameTitleScene :public GameBaseScene
{
public:
	//������
	void Initialize() override;

	//�I��
	void Finalize()override;

	//�X�V
	void Update()override;

	//�`��
	void Draw()override;


public://�����o�ϐ�

	ImguiManager* imguiManager = nullptr;	//imgui�p
	Input* input = nullptr;

	//�Q�[�����Ŏg�p����ϐ��܂Ƃ�

	uint32_t titleTexture = 0;	//�w�i�摜(���݂̓_�~�[
	Sprite* titleSprite = nullptr;

	uint32_t uiButtonATexture = 0;
	Sprite* uiButtonASprite = nullptr;

	uint32_t uiStageSelectTexture = 0;
	Sprite* uiStageSelectSprite = nullptr;

	uint32_t uiStageNumberTexture[11] = {};
	Sprite* uiStageNumberSprite[11] = {};

	uint32_t sceneChangeTexture[2]{};
	Sprite* sceneChangeSprite[2]{};

	uint32_t  titleBackTexture[4]{};
	Sprite* titleBackSprite[4]{};

	//�A�j���[�V�����p�̕ϐ��Q
	//�X�e�[�W�Z���N�g�̊�{�ʒu(�ǐ^�񒆂ƃC�[�W���O���̏㉺�̐���Ɏg��)
	DirectX::XMFLOAT2 stageBasePos{};
	DirectX::XMFLOAT2 stagePos[StageSelectPosCount]{};

	//�^�C�g���ҋ@���ƃX�e�[�W�Z���N�g�ɂȂ��ď�ɐ�����΂��ꂽ���̍��W
	DirectX::XMFLOAT2 titleSpritePos[2]{};

	EaseingData stageNumEase;
	EaseingData phaseChangeEase;

	bool isLeftDown = false;

	int phase = 0;

};

