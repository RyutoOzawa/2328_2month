#include "KEngineFramework.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
	HRESULT result;
	//windowsAPI����������
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	// DirectX����������
	directX = new ReDirectX();
	directX->Initialize(windowsAPI);

	//�L�[�{�[�h����������
	input = new Input();
	input->Initialize(windowsAPI);

	//�e�N�X�`���}�l�[�W���[�̏�����
	Texture::Initialize(directX->GetDevice());

	//�X�v���C�g���ʕ��̏�����
	spriteManager = new SpriteManager;
	spriteManager->Initialize(directX, WindowsAPI::winW, WindowsAPI::winH);

	//3D�I�u�W�F�N�g�̏�����
	Object3d::StaticInitialize(directX);

	//�J�����N���X������
	Camera::StaticInitialize(directX->GetDevice());


	//�I�[�f�B�I������
	AudioManager::StaticInitialize();

}

void KEngineFramework::Finalize()
{
	windowsAPI->Finalize();

	delete windowsAPI;
	delete input;
	delete directX;
	delete spriteManager;


}

void KEngineFramework::Update()
{
	//windows�̃��b�Z�[�W����
	if (windowsAPI->ProcessMessage()) {
		//���[�v�𔲂���
		endRequest = true;
	}

	//���͌n�X�V
	input->Update();
}

void KEngineFramework::Run()
{
	//������
	Initialize();

	//�Q�[�����[�v
	while (true)
	{
		//���t���[���X�V
		Update();
		if (IsEndRequest()) {
			break;
		}
		//�`��
		Draw();
	}

	//�I������
	Finalize();
}
