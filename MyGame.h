#pragma once
#include"KEngineFramework.h"

#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"


class MyGame : public KEngineFramework
{
public://�����o�֐�

	MyGame();

	//������
	void Initialize() override;

	//�I��
	void Finalize()override;

	//���t���[���X�V
	void Update()override;

	//�`��
	void Draw()override;

public://�����o�֐�


	//////////////////////////////////////////////

	ImguiManager* imguiManager = nullptr;	//imgui�p

	//�Q�[�����Ŏg�p����ϐ��܂Ƃ�
	uint32_t magnetTextureN = 0;	//N�ɂ̃e�N�X�`��
	uint32_t magnetTextureS = 0;	//S�ɂ̃e�N�X�`��
	uint32_t groundTexture = 0;		//�n�ʂ̃e�N�X�`��
	uint32_t playerTexture = 0;		//�v���C���[�̃e�N�X�`��

	Object3d blockObj[10][10][10];	//�u���b�N(�}�b�v�̃I�u�W�F�N�g)
	
	Player* player = nullptr;		//�v���C���[

	std::vector<MagnetBlock> magnetBlocks;	//���΃I�u�W�F�N�g
	std::vector<MagnetData> magnetDatas;	//���΃f�[�^�i�[�p

	Colision* colision = nullptr;			//�����蔻��Ǘ��N���X

	Camera camera{};						//�J����

	Map* map_ = nullptr;					//�}�b�v���

};

