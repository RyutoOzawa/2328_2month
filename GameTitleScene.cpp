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
	uiButtonATexture = Texture::LoadTexture(L"Resources/dummyTitleUI_buttonA.png");
	uiStageSelectTexture = Texture::LoadTexture(L"Resources/dummyTitleUI_SelectStage.png");
	uiStageNumberTexture[0] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber1.png");
	uiStageNumberTexture[1] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber2.png");
	uiStageNumberTexture[2] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber3.png");
	uiStageNumberTexture[3] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber4.png");
	uiStageNumberTexture[4] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber5.png");
	uiStageNumberTexture[5] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber6.png");
	uiStageNumberTexture[6] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber7.png");
	uiStageNumberTexture[7] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber8.png");
	uiStageNumberTexture[8] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber9.png");
	uiStageNumberTexture[9] = Texture::LoadTexture(L"Resources/dummyUI_stageNumber10.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

	uiButtonASprite = new Sprite();
	uiButtonASprite->Initialize(uiButtonATexture);
	uiButtonASprite->SetAnchorPoint({ 0.5f, 0.5f });
	uiButtonASprite->SetPos({ WindowsAPI::winW / 2,620.0f });
	uiButtonASprite->Update();
	uiStageSelectSprite = new Sprite();
	uiStageSelectSprite->Initialize(uiStageSelectTexture);
	uiStageSelectSprite->SetAnchorPoint({ 0.5f, 0.5f });
	uiStageSelectSprite->SetPos({ WindowsAPI::winW / 2,620.0f });
	uiStageSelectSprite->Update();

	for (int i = 0; i < _countof(uiStageNumberSprite); i++) {
		uiStageNumberSprite[i] = new Sprite();
		uiStageNumberSprite[i]->Initialize(uiStageNumberTexture[i]);
		uiStageNumberSprite[i]->SetAnchorPoint({0.5f,0.5f});
		uiStageNumberSprite[i]->SetPos({WindowsAPI::winW/2,WindowsAPI::winH/2});
		uiStageNumberSprite[i]->Update();
	}

	//�t�F�[�Y�����ʃf�[�^���玝���Ă���
	phase = ShareData::titlePhase;
}

void GameTitleScene::Finalize()
{
	//�Q�[�����[�v�Ŏg�p�������������A��ՃV�X�e���̌㏈���Ɖ�����s��

	//delete sprite;
	//delete skyDome;

	delete titleSprite;
	delete uiButtonASprite;
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
		ImGui::Text("window position leftTop : %f,%f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
		ImGui::Text("window position center  : %f,%f", ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x/2), ImGui::GetWindowPos().y+(ImGui::GetWindowSize().y / 2));

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

	if (phase == WaitInputSpaceKey) {
		uiButtonASprite->Draw();
	}
	else if (phase == StageSelect) {
		uiStageSelectSprite->Draw();
		for (int i = 0; i < _countof(uiStageNumberSprite); i++) {
			uiStageNumberSprite[ShareData::stageNumber]->Draw();
		}
	}

}
