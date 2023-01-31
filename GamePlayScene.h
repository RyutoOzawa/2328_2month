#pragma once
#include"GameBaseScene.h"
#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"ImguiManager.h"
#include"Input.h"
#include"Sprite.h"

class GamePlayScene:public GameBaseScene
{
public:
	//������
	void Initialize()override;

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
	uint32_t magnetTextureN = 0;	//N�ɂ̃e�N�X�`��
	uint32_t magnetTextureS = 0;	//S�ɂ̃e�N�X�`��
	uint32_t groundTexture = 0;		//�n�ʂ̃e�N�X�`��
	uint32_t playerTexture = 0;		//�v���C���[�̃e�N�X�`��

	uint32_t backGroundTexture = 0;	//�w�i�摜(���݂̓_�~�[
	Sprite* backGroundSprite = nullptr;

	Object3d blockObj[blockY][blockX][blockZ];	//�u���b�N(�}�b�v�̃I�u�W�F�N�g)

	Player* player = nullptr;		//�v���C���[

	std::vector<MagnetBlock> magnetBlocks;	//���΃I�u�W�F�N�g
	std::vector<MagnetData> magnetDatas;	//���΃f�[�^�i�[�p

	Colision* colision = nullptr;			//�����蔻��Ǘ��N���X

	Camera camera{};						//�J����

	Map* map_ = nullptr;					//�}�b�v���



};

