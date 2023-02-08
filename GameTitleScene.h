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

	int phase = 0;

};

