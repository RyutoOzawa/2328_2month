#include "KEngineFramework.h"

//KEngineFramework::KEngineFramework()
//{
//}

void KEngineFramework::Initialize()
{
}

void KEngineFramework::Finalize()
{
}

void KEngineFramework::Update()
{
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
