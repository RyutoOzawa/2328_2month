#include "GameTitleScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
using namespace DirectX;
#include"GameSceneManager.h"
#include"ShareData.h"

void GameTitleScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//テクスチャデータ初期化
	titleTexture = Texture::LoadTexture(L"Resources/dummyTitle.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

	//フェーズをスペースキー押し待ち状態に
	phase = WaitInputSpaceKey;
}

void GameTitleScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

	delete titleSprite;
	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GameTitleScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//


	ImGui::Begin("debug");
	if (phase == WaitInputSpaceKey) {
		ImGui::Text("phase:WaitInputSpaceKey");
		ImGui::Text("PUSH PAD A GO TO STAGE SELECT");

		ImGui::Text("this window size: %f,%f", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		//スペースキーでステージ選択へ
		if (input->IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			phase = StageSelect;
		}
	}
	else if (phase == StageSelect) {

		ImGui::Text("phase:StageSelect");
		ImGui::Text("select stage to LEFT or RIGHT ");
		ImGui::Text("stageNumber %d",ShareData::stageNumber);

		//左右キーでステージ番号変更
		if (input->IsTriggerLStickLeft()) {
			ShareData::stageNumber--;
		}
		else if (input->IsTriggerLStickRight()) {
			ShareData::stageNumber++;
		}

		if (ShareData::stageNumber < Sample1)ShareData::stageNumber = Sample1;
		else if (ShareData::stageNumber >= StageIndexCount)ShareData::stageNumber = tutorial1;

		if (input->IsPadTrigger(XINPUT_GAMEPAD_A)) {
			//シーンの切り替えを依頼
			sceneManager->ChangeScene("GAMEPLAY");
		}
	}


	ImGui::End();
	//----------------------ゲーム内ループはここまで---------------------//


}

void GameTitleScene::Draw()
{
	//-------スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	titleSprite->Draw();

}
