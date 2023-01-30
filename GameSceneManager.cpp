#include "GameSceneManager.h"
#include"GameTitleScene.h"

GameSceneManager::GameSceneManager()
{
	//�ŏ��̓^�C�g���V�[��
	GameBaseScene* firstScene = new GameTitleScene();
	SetNextScene(firstScene);
}

GameSceneManager::~GameSceneManager()
{
	//���݂̃V�[���̏I���ƊJ��
	activeScene->Finalize();
	delete activeScene;
}

GameSceneManager* GameSceneManager::GetInstance()
{
	static GameSceneManager instance;
	return &instance;
}

void GameSceneManager::Update()
{
	
	//���̃V�[�����\�񂳂�Ă���Ȃ�
	if (nextScene) {
		//���V�[���I��
		if (activeScene) {
			activeScene->Finalize();
			delete activeScene;
		}

		//�V�[���؂�ւ�
		activeScene = nextScene;
		nextScene = nullptr;

		//�V�[��������V�[���؂�ւ����˗��ł���悤�ɁA�V�[���}�l�[�W�����Z�b�g����
		activeScene->SetSceneManager(this);

		//���V�[��������
		activeScene->Initialize();

	}
	//���s�V�[���̍X�V
	activeScene->Update();
}

void GameSceneManager::Draw()
{
	activeScene->Draw();
}
