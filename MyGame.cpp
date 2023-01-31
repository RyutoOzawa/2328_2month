#include "MyGame.h"
using namespace DirectX;
#include"SceneFactory.h"

MyGame::MyGame()
{
}

void MyGame::Initialize()
{
	//���N���X�̏���������
	KEngineFramework::Initialize();

	//�V�[���t�@�N�g���[�̐����ƃ}�l�[�W���փZ�b�g
	sceneFactory = new SceneFactory();
	GameSceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	//�V�[���}�l�[�W���ɍŏ��̃V�[�����Z�b�g
	GameSceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Finalize()
{
	// ���N���X�̏I������
	KEngineFramework::Finalize();
}

void MyGame::Update()
{
	//imgui�J�n����
	imguiManager->Begin();

	//���N���X�̍X�V����
	KEngineFramework::Update();

	//imgui�I��
	imguiManager->End();


}

void MyGame::Draw()
{
	directX->BeginDraw();

	//�V�[���`�揈��
	sceneManager->Draw();

	imguiManager->Draw();
	directX->EndDraw();
}
