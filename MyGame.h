#pragma once
#include"KEngineFramework.h"
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

	//�V�X�e����Ղ̕ϐ��܂Ƃ�

	WindowsAPI* windowsAPI = nullptr;
	ReDirectX* directX = nullptr;
	Input* input = nullptr;
	SpriteManager* spriteManager = nullptr;
	ImguiManager* imguiManager = nullptr;
	bool endRequest = false;

	//////////////////////////////////////////////

	//�Q�[�����Ŏg�p����ϐ��܂Ƃ�
	uint32_t magnetTextureN = 0;
	uint32_t magnetTextureS = 0;
	uint32_t groundTexture = 0;
	uint32_t playerTexture = 0;

	Object3d blockObj[10][10][10];

	Player* player = nullptr;

	std::vector<MagnetBlock> magnetBlocks;
	std::vector<MagnetData> magnetDatas;

	Colision* colision = nullptr;

	Camera camera{};

	Map* map_ = nullptr;

};

