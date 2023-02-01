#include "GameTitleScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
using namespace DirectX;
#include"GameSceneManager.h"
#include"ShareData.h"

void GameTitleScene::Initialize()
{


	//--------------�Q�[�����ϐ�������--------------//

	//input�̃C���X�^���X�擾
	input = Input::GetInstance();

	//�e�N�X�`���f�[�^������
	titleTexture = Texture::LoadTexture(L"Resources/dummyTitle.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

	//�t�F�[�Y���X�y�[�X�L�[�����҂���Ԃ�
	phase = WaitInputSpaceKey;
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


	ImGui::Begin("debug");
	if (phase == WaitInputSpaceKey) {
		ImGui::Text("phase:WaitInputSpaceKey");
		ImGui::Text("PUSH PAD A GO TO STAGE SELECT");

		ImGui::Text("this window size: %f,%f", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		//�X�y�[�X�L�[�ŃX�e�[�W�I����
		if (input->IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			phase = StageSelect;
		}
	}
	else if (phase == StageSelect) {

		ImGui::Text("phase:StageSelect");
		ImGui::Text("select stage to LEFT or RIGHT ");
		ImGui::Text("stageNumber %d",ShareData::stageNumber);

		//���E�L�[�ŃX�e�[�W�ԍ��ύX
		if (input->IsTriggerLStickLeft()) {
			ShareData::stageNumber--;
		}
		else if (input->IsTriggerLStickRight()) {
			ShareData::stageNumber++;
		}

		if (ShareData::stageNumber < Sample1)ShareData::stageNumber = Sample1;
		else if (ShareData::stageNumber >= StageIndexCount)ShareData::stageNumber = tutorial1;

		if (input->IsPadTrigger(XINPUT_GAMEPAD_A)) {
			//�V�[���̐؂�ւ����˗�
			sceneManager->ChangeScene("GAMEPLAY");
		}
	}


	ImGui::End();
	//----------------------�Q�[�������[�v�͂����܂�---------------------//


}

void GameTitleScene::Draw()
{
	//-------�X�v���C�g�`�揈��-------//
	SpriteManager::GetInstance()->beginDraw();

	titleSprite->Draw();

}
