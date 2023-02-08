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

	//各種スプライト座標初期化
	stageBasePos = { WindowsAPI::winW / 2,WindowsAPI::winH / 2 };


	stagePos[LeftLeft] = { -WindowsAPI::winW / 2,stageBasePos.y };
	stagePos[Left] = { 0,stageBasePos.y };
	stagePos[Center] = stageBasePos;
	stagePos[Right] = { WindowsAPI::winW,stageBasePos.y };
	stagePos[RightRight] = { WindowsAPI::winW + WindowsAPI::winW / 2,stageBasePos.y };

	//音
	titleBGM = new AudioManager();
	titleBGM->SoundLoadWave("Resources/Audio/titleBGM.wav");

	decisionSE = new AudioManager();
	decisionSE->SoundLoadWave("Resources/Audio/decisionSE.wav");

	serectSE = new AudioManager();
	serectSE->SoundLoadWave("Resources/Audio/serectSE.wav");


	//テクスチャデータ初期化
	titleTexture = Texture::LoadTexture(L"Resources/titleRogo.png");
	uiButtonATexture = Texture::LoadTexture(L"Resources/UI/pushA.png");
	uiStageSelectTexture = Texture::LoadTexture(L"Resources/UI/stageSerectUI/stageSerect.png");

	uiStageNumberTexture[0] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/tuto.png");
	uiStageNumberTexture[1] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/1.png");
	uiStageNumberTexture[2] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/2.png");
	uiStageNumberTexture[3] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/3.png");
	uiStageNumberTexture[4] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/4.png");
	uiStageNumberTexture[5] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/5.png");
	uiStageNumberTexture[6] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/6.png");
	uiStageNumberTexture[7] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/7.png");
	uiStageNumberTexture[8] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/8.png");
	uiStageNumberTexture[9] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/9.png");
	uiStageNumberTexture[10] = Texture::LoadTexture(L"Resources/UI/stageSerectUI/10.png");

	sceneChangeTexture[0] = Texture::LoadTexture(L"Resources/magnetN.png");
	sceneChangeTexture[1] = Texture::LoadTexture(L"Resources/magnetS.png");

	titleBackTexture[0] = Texture::LoadTexture(L"Resources/gameBack/gameBack.png");
	titleBackTexture[1] = Texture::LoadTexture(L"Resources/gameBack/gameBackStar1.png");
	titleBackTexture[2] = Texture::LoadTexture(L"Resources/gameBack/gameBackStar2.png");
	titleBackTexture[3] = Texture::LoadTexture(L"Resources/gameBack/gameBackStar3.png");

	titleSprite = new Sprite();
	titleSprite->Initialize(titleTexture);

	titleSprite->SetAnchorPoint({ 0.5f,0.5f });
	titleSprite->SetPos({ WindowsAPI::winW / 2,WindowsAPI::winH / 2 - 100 });
	titleSprite->Update();

	for (int i = 0; i < _countof(titleBackSprite); i++) {
		titleBackSprite[i] = new Sprite();
		titleBackSprite[i]->Initialize(titleBackTexture[i]);
	}

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
		uiStageNumberSprite[i]->SetAnchorPoint({ 0.5f,0.5f });
		uiStageNumberSprite[i]->SetPos({ WindowsAPI::winW / 2,WindowsAPI::winH / 2 });
		//uiStageNumberSprite[i]->SetSize(XMFLOAT2(160.0f,160.0f));
		uiStageNumberSprite[i]->Update();
	}



	//シーンチェンジ用の変数
	for (int i = 0; i < _countof(sceneChangeSprite); i++) {
		ShareData::nextPos[i] = ShareData::easePos[i][1];
		sceneChangeSprite[i] = new Sprite();
		sceneChangeSprite[i]->Initialize(sceneChangeTexture[i]);
		sceneChangeSprite[i]->SetSize({ WindowsAPI::winH,WindowsAPI::winH });
		sceneChangeSprite[i]->SetPos(ShareData::easePos[i][1]);
	}

	//直前のシーンでシーンクローズ処理が行われたかチェック(このシーンの初期化がexe起動かどうかのチェック)
	if (ShareData::isBeforeSceneClosed) {
		ShareData::OpenSceneChange();
	}

	sceneChangeSprite[0]->SetAnchorPoint({ 1.0f,0.0f });


	//フェーズを共通データから持ってくる
	phase = ShareData::titlePhase;


}

void GameTitleScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;

	delete titleSprite;
	delete uiButtonASprite;
	delete uiStageSelectSprite;

	for (int i = 0; i < 4; i++) {
		delete titleBackSprite[i];
	}


	for (int i = 0; i < 10; i++) {
		delete uiStageNumberSprite[i];
	}

	for (int i = 0; i < 2; i++) {
		delete	sceneChangeSprite[i];
	}
	//-------------ここまでにループ内で使用したものの後処理------------//



}

void GameTitleScene::Update()
{


	//----------------------ゲーム内ループはここから---------------------//


	//タイトルBGM流す
	titleBGM->SoundPlayWave(true, titleBGMVolume);


	if (phase == WaitInputSpaceKey) {



		//スペースキーでステージ選択へ
		if (input->IsPadTrigger(XINPUT_GAMEPAD_A) || input->IsKeyTrigger(DIK_SPACE))
		{
			phase = StageSelect;
			decisionSE->StopWave();
			decisionSE->SoundPlayWave(false,decisionSEVolume);
		}
	}
	else if (phase == StageSelect) {
		ImGui::Begin("stage select Animation");
		ImGui::Text("timerate %f", stageNumEase.timeRate);

		ImGui::End();
		//左右キーでステージ番号変更

		if (input->IsTriggerLStickLeft() || input->IsKeyTrigger(DIK_A)) {
			ShareData::stageNumber--;
			serectSE->StopWave();
			serectSE->SoundPlayWave(false, serectSEVolume);
		}
		else if (input->IsTriggerLStickRight() || input->IsKeyTrigger(DIK_D)) {
			ShareData::stageNumber++;
			serectSE->StopWave();
			serectSE->SoundPlayWave(false, serectSEVolume);
		}


		//ステージ番号移動のイージングが終わっているなら操作可能
		if (!stageNumEase.GetActive()) {

			if (input->IsTriggerLStickLeft() || input->IsKeyTrigger(DIK_A)) {
				ShareData::stageNumber--;
				stageNumEase.Start(0.5f);
				isLeftDown = true;
			}
			else if (input->IsTriggerLStickRight() || input->IsKeyTrigger(DIK_D)) {
				ShareData::stageNumber++;
				stageNumEase.Start(0.5f);
				isLeftDown = false;
			}


		if (input->IsPadTrigger(XINPUT_GAMEPAD_A) || input->IsKeyTrigger(DIK_SPACE)) {
			//シーンの切り替えを依頼
			ShareData::CloseSceneChange();
			decisionSE->StopWave();
			decisionSE->SoundPlayWave(false, decisionSEVolume);
		}

		//Bボタンでタイトルへ
		if (input->IsPadTrigger(XINPUT_GAMEPAD_B) || input->IsKeyTrigger(DIK_B)) {
			phase = WaitInputSpaceKey;
			decisionSE->StopWave();
			decisionSE->SoundPlayWave(false,decisionSEVolume);
		}

			if (ShareData::stageNumber < Tutoattract)ShareData::stageNumber = Tutoattract;
			else if (ShareData::stageNumber >= StageIndexCount)ShareData::stageNumber = Mislead;

		//シーンクローズフラグが立っていて、シーンチェンジフラグが降りている(アニメーションが終了した)ならシーン切替を依頼
		if (ShareData::isBeforeSceneClosed && !ShareData::isActiveSceneChange) {
			sceneManager->ChangeScene("GAMEPLAY");
			titleBGM->StopWave();
		}
	}

	//ステージ番号イージングの更新
	stageNumEase.Update();

	//ステージ番号のY座標は常にbaseに追従
	for (int i = 0; i < StageSelectPosCount; i++) {
		stagePos[i].y = stageBasePos.y;
	}
	ImGui::Begin("spritepos");


	//ステージ番号の移動処理
	XMFLOAT2 easeStagePos;
	for (int i = 0; i < StageIndexCount; i++) {
		//スティックが左に倒された(ステージの選択番号が下がった)なら
		if (stageNumEase.GetActive()) {
			if (!isLeftDown) {
				//ステージインデックスとステージナンバーが一致している(操作で選択された番号)ならposがcenterになるように
				if (i == ShareData::stageNumber) {
					//左に倒されているので座標は右端から中央へ動く(right→center)
					easeStagePos = EaseOut2D(stagePos[Right], stagePos[Center], stageNumEase.timeRate);
				}//インデックスが1小さい(選択番号の左となり)は中央から左端へ
				else if (i == ShareData::stageNumber - 1) {
					easeStagePos = EaseOut2D(stagePos[Center], stagePos[Left], stageNumEase.timeRate);
				}//インデックスが1大きい(選択番号の右となり)は右側画面外から右端
				else if (i == ShareData::stageNumber + 1) {
					easeStagePos = EaseOut2D(stagePos[RightRight], stagePos[Right], stageNumEase.timeRate);
				}//インデックスが2小さいやつは左端から左側画面外へ(アニメーションの整合性を保つ)
				else if (i == ShareData::stageNumber - 2) {
					easeStagePos = EaseOut2D(stagePos[Left], stagePos[LeftLeft], stageNumEase.timeRate);
				}//それ以外でインデックスより数が若いものは左側画面外で固定(値が変動しない)
				else if (i > ShareData::stageNumber) {
					easeStagePos = stagePos[LeftLeft];
				}//それ以外でインデックスより大きい数は右側画面外で固定(値が変動しない)
				else if (i < ShareData::stageNumber) {
					easeStagePos = stagePos[RightRight];
				}
			}//こっちはスティックが右に倒されたパターン(数が上がったのでindex誤差+-2の部分が逆になる)
			else {
				//ステージインデックスとステージナンバーが一致している(操作で選択された番号)ならposがcenterになるように
				if (i == ShareData::stageNumber) {
					easeStagePos = EaseOut2D(stagePos[Left], stagePos[Center], stageNumEase.timeRate);
				}
				else if (i == ShareData::stageNumber - 1) {
					easeStagePos = EaseOut2D(stagePos[LeftLeft], stagePos[Left], stageNumEase.timeRate);
				}
				else if (i == ShareData::stageNumber + 1) {
					easeStagePos = EaseOut2D(stagePos[Center], stagePos[Right], stageNumEase.timeRate);
				}
				else if (i == ShareData::stageNumber + 2) {
					easeStagePos = EaseOut2D(stagePos[Right], stagePos[RightRight], stageNumEase.timeRate);
				}
				else if (i > ShareData::stageNumber) {
					easeStagePos = stagePos[LeftLeft];
				}
				else if (i < ShareData::stageNumber) {
					easeStagePos = stagePos[RightRight];
				}
			}
		}
		else {
			//イージングが起きていない時
			if (i == ShareData::stageNumber) {
				easeStagePos = stagePos[Center];
			}
			else if (i == ShareData::stageNumber+1) {
				easeStagePos = stagePos[Right];
			}
			else if (i == ShareData::stageNumber - 1) {
				easeStagePos = stagePos[Left];
			}
			else {
				easeStagePos = stagePos[LeftLeft];
			}
		}

		//変動した座標をセット
		uiStageNumberSprite[i]->SetPos(easeStagePos);
		ImGui::Text("pos[%d] %f,%f",i, uiStageNumberSprite[i]->GetPosition().x, uiStageNumberSprite[i]->GetPosition().y);
	}


	ImGui::End();

	//シーンチェンジ用の更新はフェーズを問わず行う
	//イージングタイマー制御用の更新
	ShareData::sceneChangeEase.Update();

	for (int i = 0; i < 2; i++) {
		if (!ShareData::sceneChangeEase.GetActive()) {
			sceneChangeSprite[i]->SetPos(ShareData::nextPos[i]);
			ShareData::isActiveSceneChange = false;
		}
		else {
			XMFLOAT2 spritePos = EaseIn2D(ShareData::easePos[i][1], ShareData::easePos[i][0],
				ShareData::sceneChangeEase.timeRate);
			sceneChangeSprite[i]->SetPos(spritePos);

		}
		sceneChangeSprite[i]->Update();
	}



	sinAngle += 2.0f;
	if (sinAngle > 360.0f)sinAngle -= 360.0f;
	ImGui::Text("angle %f", sinAngle);
	//背景の星を点滅させる
	for (int i = 1; i < _countof(titleBackSprite); i++) {
		titleBackSprite[i]->color.w = (sin((sinAngle + i * 120.0f) * XM_PI / 180.0f)) / 2.0f + 0.5f;
		ImGui::Text("alpha[%d] %f", i, titleBackSprite[i]->color.w);
	}


	//----------------------ゲーム内ループはここまで---------------------//


}

void GameTitleScene::Draw()
{
	//-------スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	for (int i = 0; i < _countof(titleBackSprite); i++) {
		titleBackSprite[i]->Draw();
	}

	if (phase == WaitInputSpaceKey) {
		titleSprite->Draw();
	}

	

	if (phase == WaitInputSpaceKey) {
		uiButtonASprite->Draw();
	}
	else if (phase == StageSelect) {
		uiStageSelectSprite->Draw();
		for (int i = 0; i < _countof(uiStageNumberSprite); i++) {
			if (i <= ShareData::stageNumber + 2 && i >= ShareData::stageNumber - 2) {
				uiStageNumberSprite[i]->Draw();
			}
		}
	}

	for (int i = 0; i < _countof(sceneChangeSprite); i++) {
		sceneChangeSprite[i]->Draw();
	}

}
