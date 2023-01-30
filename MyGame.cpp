#include "MyGame.h"
using namespace DirectX;



MyGame::MyGame()
{
}

void MyGame::Initialize()
{
	//���N���X�̏���������
	KEngineFramework::Initialize();
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
