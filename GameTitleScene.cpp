#include "GameTitleScene.h"
#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
using namespace DirectX;
#include"GameSceneManager.h"

void GameTitleScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������
	titleTexture = Texture::LoadTexture(L"Resources/dummyTitle.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

}

void GameTitleScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;

	delete titleSprite;
	//-------------�����܂łɃ��[�v���Ŏg�p�������̂̌㏈��------------//



}

void GameTitleScene::Update()
{


	//----------------------�Q�[�������[�v�͂�������---------------------//

	//�X�y�[�X�L�[�Ń��C���Q�[����
	if (input->IsKeyTrigger(DIK_SPACE))
	{
		//�Q�[���V�[������
		GameBaseScene* newScene = new GamePlayScene();
		//�V�[���̐؂�ւ����˗�
		sceneManager->SetNextScene(newScene);
	}

	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GameTitleScene::Draw()
{
	//-------�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	titleSprite->Draw();

}
