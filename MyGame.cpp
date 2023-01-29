#include "MyGame.h"
using namespace DirectX;



MyGame::MyGame()
{
}

void MyGame::Initialize()
{
	//���N���X�̏���������
	KEngineFramework::Initialize();


	//�Q�[���V�[���̐����Ə�����
	gameScene = new GamePlayScene();
	gameScene->Initialize();
	
}

void MyGame::Finalize()
{
	//�V�[���̏I���ƊJ��
	gameScene->Finalize();
	delete gameScene;

	// ���N���X�̏I������
	KEngineFramework::Finalize();
}

void MyGame::Update()
{
	//���N���X�̍X�V����
	KEngineFramework::Update();

	//imgui�J�n����
	imguiManager->Begin();

	//�V�[���X�V����
	gameScene->Update();

	//imgui�I��
	imguiManager->End();


}

void MyGame::Draw()
{
	directX->BeginDraw();

	gameScene->Draw();

	imguiManager->Draw();
	directX->EndDraw();
}
