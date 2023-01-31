#pragma once
#include"WindowsAPI.h"
#include"DirectX.h"
#include"Input.h"
#include"Texture.h"
#include"SpriteManager.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Camera.h"
#include"ImguiManager.h"
#include"AudioManager.h"
#include"Player.h"
#include"Map.h"
#include"MagnetBlock.h"
#include"Colision.h"
#include"Goal.h"

class MyGame
{
public://�����o�֐�
	//������
	void Initialize();

	//�I��
	void Finalize();

	//���t���[���X�V
	void Update();

	//�`��
	void Draw();

	//�I���t���O�`�F�b�N
	bool IsEndReqest() { return endRequest; }

public://�����o�֐�

	//�V�X�e����Ղ̕ϐ��܂Ƃ�

	WindowsAPI* windowsAPI;
	ReDirectX* directX;
	Input* input;
	SpriteManager* spriteManager;
	ImguiManager* imguiManager;
	bool endRequest = false;

	//////////////////////////////////////////////

	//�Q�[�����Ŏg�p����ϐ��܂Ƃ�
	uint32_t magnetTextureN = 0;
	uint32_t magnetTextureS = 0;
	uint32_t groundTexture = 0;
	uint32_t playerTexture = 0;
	uint32_t goalTexture = 0;
	uint32_t clearTexture = 0;

	Object3d blockObj[10][10][10];

	Player* player;

	std::vector<MagnetBlock> magnetBlocks;
	std::vector<MagnetData> magnetDatas;

	Colision* colision;

	Camera camera;

	Map* map_;

	Goal* goal;
	Sprite goalSprite;

};

