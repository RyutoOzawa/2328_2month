#pragma once
#include"KEngineFramework.h"
#include"GamePlayScene.h"



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

	//�V�[��
	GamePlayScene* gameScene = nullptr;
};

