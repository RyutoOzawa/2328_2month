#pragma once
#include"GameBaseScene.h"

class GameSceneManager
{
private:
	GameSceneManager();
	~GameSceneManager();

public:

	//�R�s�[�R���X�g���N�^����
	GameSceneManager(const GameSceneManager& obj) = delete;
	//������Z�q�𖳌�
	GameSceneManager& operator=(const GameSceneManager& obj) = delete;

	//�C���X�^���X�A�N�Z�X��p�֐�
	static GameSceneManager* GetInstance();

	//�X�V
	void Update();

	//�`��
	void Draw();


	void SetNextScene(GameBaseScene* nextScene) { this->nextScene = nextScene; }

private:
	//���݂̃V�[��
	GameBaseScene* activeScene = nullptr;
	GameBaseScene* nextScene = nullptr;


};

