#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"ShareData.h"
#include"Util.h"

using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	blockModel = Model::CreateModel("cube");

	//ブロック
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			for (int k = 0; k < 20; k++) {
				blockObj[i][j][k].Initialize();

				blockObj[i][j][k].SetModel(blockModel);
			}
		}
	}


	//テクスチャデータ初期化
	magnetTextureN = Texture::LoadTexture(L"Resources/magnetN.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/magnetS.png");
	groundTexture = Texture::LoadTexture(L"Resources/testGround.png");

	groundTextures[0] = Texture::LoadTexture(L"Resources/groundPattern1.png");
	groundTextures[1] = Texture::LoadTexture(L"Resources/groundPattern2.png");
	groundTextures[2] = Texture::LoadTexture(L"Resources/groundPattern3.png");
	groundTextures[3] = Texture::LoadTexture(L"Resources/groundPattern4.png");
	playerTexture = Texture::LoadTexture(L"Resources/white.png");
	backGroundTexture = Texture::LoadTexture(L"Resources/gameBack/gameBack1.png");
	clearTexture = Texture::LoadTexture(L"Resources/UI/clearUI/clear.png");
	clearNextTexture = Texture::LoadTexture(L"Resources/UI/clearUI/clearNext.png");
	clearStageSerectTexture = Texture::LoadTexture(L"Resources/UI/clearUI/clearStageSerect.png");

	menuTexture = Texture::LoadTexture(L"Resources/UI/menuUI/menu.png");
	menuResetTexture = Texture::LoadTexture(L"Resources/UI/menuUI/menuReset.png");
	menuTitleTexture = Texture::LoadTexture(L"Resources/UI/menuUI/menuTitle.png");
	menuStageSerectTexture = Texture::LoadTexture(L"Resources/UI/menuUI/menuStageSerect.png");


	playerTextureN = Texture::LoadTexture(L"Resources/playerN.png");
	playerTextureS = Texture::LoadTexture(L"Resources/playerS.png");

	//selectBoxTexture = Texture::LoadTexture(L"Resources/dummyUI_selectBox.png");
	//selectBoxSprite = new Sprite();
	//selectBoxSprite->Initialize(selectBoxTexture);
	//selectBoxSprite->SetAnchorPoint({ 0.5f,0.5f });
	//selectBoxSprite->Update();
	//boxPos = selectBoxSprite->GetPosition();

	playUITexture = Texture::LoadTexture(L"Resources/UI/playGameUI.png");
	playUISprite = new Sprite();
	playUISprite->Initialize(playUITexture);
	playUISprite->SetAnchorPoint({ 0.0f,1.0f });
	playUISprite->SetPos({ 0.0f, WindowsAPI::winH });
	playUISprite->Update();

	backGroundSprite = new Sprite();
	backGroundSprite->Initialize(backGroundTexture);

	//メニューUI

	menuSprite = new Sprite();
	menuSprite->Initialize(menuTexture);
	//アンカーポイントをスプライトの中心に
	menuSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	menuSprite->SetPos(XMFLOAT2(WindowsAPI::winW / 2, WindowsAPI::winH / 2));
	menuSprite->Update();

	//Reset

	menuResetSprite = new Sprite();
	menuResetSprite->Initialize(menuResetTexture);
	//アンカーポイントをスプライトの中心に
	menuResetSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	menuResetSprite->SetPos(XMFLOAT2(WindowsAPI::winW / 2, WindowsAPI::winH / 2 - 40));
	menuResetSprite->Update();

	//Title

	menuTitleSprite = new Sprite();
	menuTitleSprite->Initialize(menuTitleTexture);
	//アンカーポイントをスプライトの中心に
	menuTitleSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	menuTitleSprite->SetPos(XMFLOAT2(WindowsAPI::winW / 2, WindowsAPI::winH / 2 + 120));
	menuTitleSprite->Update();

	//StageSerect

	menuStageSerectSprite = new Sprite();
	menuStageSerectSprite->Initialize(menuStageSerectTexture);
	//アンカーポイントをスプライトの中心に
	menuStageSerectSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	menuStageSerectSprite->SetPos(XMFLOAT2(WindowsAPI::winW / 2, WindowsAPI::winH / 2 + 40));
	menuStageSerectSprite->Update();

	//clear
	clearSprite = new Sprite();
	clearSprite->Initialize(clearTexture);
	clearSprite->SetPos(XMFLOAT2(340, 200));
	clearSprite->Update();

	clearNextSprite = new Sprite();
	clearNextSprite->Initialize(clearNextTexture);
	clearNextSprite->SetPos(XMFLOAT2(560, 350));
	clearNextSprite->Update();

	clearStageSerectSprite = new Sprite();
	clearStageSerectSprite->Initialize(clearStageSerectTexture);
	clearStageSerectSprite->SetPos(XMFLOAT2(450, 410));
	clearStageSerectSprite->Update();


	selectBoxPos[0] = { WindowsAPI::winW / 2,320.0f };
	selectBoxPos[1] = { WindowsAPI::winW / 2,400.0f };
	selectBoxPos[2] = { WindowsAPI::winW / 2,480.0f };

	//シーン遷移の初期化	//直前のシーンでシーンクローズ処理が行われたかチェック(このシーンの初期化がexe起動かどうかのチェック)
	if (ShareData::isBeforeSceneClosed) {
		ShareData::OpenSceneChange();
	}
	uint32_t sceneChangeTexture[2];
	sceneChangeTexture[0] = magnetTextureN;
	sceneChangeTexture[1] = magnetTextureS;
	//シーンチェンジ用の変数
	for (int i = 0; i < _countof(sceneChangeSprite); i++) {
		//ShareData::nextPos[i] = ShareData::easePos[i][1];
		sceneChangeSprite[i] = new Sprite();
		sceneChangeSprite[i]->Initialize(sceneChangeTexture[i]);
		sceneChangeSprite[i]->SetSize({ WindowsAPI::winH,WindowsAPI::winH });
		sceneChangeSprite[i]->SetPos(ShareData::easePos[i][1]);
	}

	sceneChangeSprite[0]->SetAnchorPoint({ 1.0f,0.0f });

	//カメラ初期化
	XMFLOAT3 eye(5, 25, 6);	//視点座標
	XMFLOAT3 target(5, 0, 6);	//注視点座標
	XMFLOAT3 up(0, 1, 0);		//上方向ベクトル

	camera.Initialize(eye, target, up);

	//マップ読み込み
	map_ = new Map;

	//ステージのリセット
	StageInitialize(ShareData::stageNumber);
}

void GamePlayScene::Finalize()
{
	//ゲームループで使用した物を解放後、基盤システムの後処理と解放を行う

	//delete sprite;
	//delete skyDome;
	delete map_;
	delete player;

	delete backGroundSprite;
	delete menuSprite;
	delete selectBoxSprite;
	delete playUISprite;
	delete colision;

	for (int i = 0; i < 2; i++) {
		delete sceneChangeSprite[i];
	}
	//-------------ここまでにループ内で使用したものの後処理------------//
}

void GamePlayScene::Update()
{

	//----------------------ゲーム内ループはここから---------------------//


	//シーンチェンジ用の更新はフェーズを問わず行う
//イージングタイマー制御用の更新
	ShareData::sceneChangeEase.Update();

	ImGui::Begin("Easing data");

	ImGui::Text("timeRate:%f", ShareData::sceneChangeEase.timeRate);
	ImGui::Text("nowCount:%lld", ShareData::sceneChangeEase.nowCount);
	ImGui::Text("startCount:%lld", ShareData::sceneChangeEase.startCount);

	ImGui::End();

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

	if (goal->isGoal) {
		//スティック上下でメニューを選ぶ
		if (input->IsTriggerLStickDown() || input->IsKeyTrigger(DIK_D)) {
			clearMenuNumber++;
		}
		else if (input->IsTriggerLStickUp() || input->IsKeyTrigger(DIK_A)) {
			clearMenuNumber--;
		}

		if (clearMenuNumber == 0) {

			clearNextSprite->color.z = sin(clock());
			clearStageSerectSprite->color = { 1,1,1,1 };
		}
		else if (clearMenuNumber == 1) {
			clearStageSerectSprite->color.z = sin(clock());
			clearNextSprite->color= { 1,1,1,1 };
		}

		//最大値、最小値を超えないように
		if (clearMenuNumber > 1)clearMenuNumber = 1;
		else if (clearMenuNumber < 0)clearMenuNumber = 0;

		//Aボタンで決定
		if (input->IsPadTrigger(XINPUT_GAMEPAD_A) || input->IsKeyTrigger(DIK_SPACE)) {
			//メニュー選択番号が0かつ、最終ステージでないならなら次のステージへ
			if (clearMenuNumber == 0 && ShareData::stageNumber < StageIndex::Mislead) {
				ShareData::stageNumber++;
				StageInitialize(ShareData::stageNumber);
			}//それ以外(ステージ選択ボタンが押されたか、ステージが一番最後)だったらステージ選択に戻る
			else {
				//共通データのフェーズを入力待ち(タイトル画面)に変更し、タイトルシーンへ戻る
				ShareData::titlePhase = TitlePhaseIndex::StageSelect;
				sceneManager->ChangeScene("TITLE");
			}

		}

	}
	else {
		if (isMenu) {

			//スタートボタンでメニューを閉じる
			if (input->IsPadTrigger(XINPUT_GAMEPAD_START) || input->IsKeyTrigger(DIK_M)) {
				isMenu = false;
			}

			//スティック上下でメニューを選ぶ
			if (input->IsTriggerLStickDown() || input->IsKeyTrigger(DIK_S)) {
				selectMenuNumber++;
			}
			else if (input->IsTriggerLStickUp() || input->IsKeyTrigger(DIK_W)) {
				selectMenuNumber--;
			}
			//メニュー選択
			if (selectMenuNumber == Reset) {
				menuResetSprite->color.z = sin(clock());
				menuTitleSprite->color = { 1,1,1,1 };
				menuStageSerectSprite->color = { 1,1,1,1 };
			}
			else if (selectMenuNumber == StageSelect_MENU) {
				menuStageSerectSprite->color.z = sin(clock());
				menuResetSprite->color = { 1,1,1,1 };
				menuTitleSprite->color = { 1,1,1,1 };
			}
			else if (selectMenuNumber == Title) {
				menuTitleSprite->color.z = sin(clock());
				menuResetSprite->color = { 1,1,1,1 };
				menuStageSerectSprite->color = { 1,1,1,1 };
			}

			//最大値、最小値を超えないように
			if (selectMenuNumber > MenuIndex::Title)selectMenuNumber = Title;
			else if (selectMenuNumber < MenuIndex::Reset)selectMenuNumber = Reset;

			if (input->IsPadTrigger(XINPUT_GAMEPAD_A) || input->IsKeyTrigger(DIK_SPACE)) {

				if (selectMenuNumber == Reset) {
					StageInitialize(ShareData::stageNumber);
				}
				else if (selectMenuNumber == StageSelect_MENU) {

					//共通データのフェーズをステージ選択に変更し、タイトルシーンへ戻る
					ShareData::titlePhase = TitlePhaseIndex::StageSelect;
					ShareData::CloseSceneChange();
				}
				else if (selectMenuNumber == Title) {

					//共通データのフェーズを入力待ち(タイトル画面)に変更し、タイトルシーンへ戻る
					ShareData::titlePhase = TitlePhaseIndex::WaitInputSpaceKey;
					ShareData::CloseSceneChange();
				}

			}

			//シーンクローズフラグが立っていて、シーンチェンジフラグが降りている(アニメーションが終了した)ならシーン切替を依頼
			if (ShareData::isBeforeSceneClosed && !ShareData::isActiveSceneChange) {
				sceneManager->ChangeScene("TITLE");
			}

			ImGui::Begin("menu");
			ImGui::Text("menuNumber %d", selectMenuNumber);
			ImGui::Text("this window size: %f,%f", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
			ImGui::Text("window position leftTop : %f,%f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
			ImGui::Text("window position center  : %f,%f", ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2), ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2));
			ImGui::SliderFloat("alpha", &menuSprite->color.w, 0.0f, 1.0f);
			ImGui::SliderFloat("boxPosX", &boxPos.x, 0.0f, WindowsAPI::winW);
			ImGui::SliderFloat("boxPosY", &boxPos.y, 0.0f, WindowsAPI::winH);
			ImGui::End();

			//selectBoxSprite->SetPos(selectBoxPos[selectMenuNumber]);
			//selectBoxSprite->Update();

		}
		else {

			//fps表示
			ImGui::Begin("fps");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			//スタートボタンでメニューへ
			if (input->IsPadTrigger(XINPUT_GAMEPAD_START) || input->IsKeyTrigger(DIK_M)) {
				isMenu = true;
				//選択は初期はリセット
				selectMenuNumber = Reset;
			}


			//磁力計算
			for (int i = 0; i < magnetDatas.size(); i++) {
				colision->UpdateDeta(player, magnetBlocks[i], i);
			}

			colision->Update();

			//座標の更新
			for (int i = 0; i < magnetBlocks.size(); i++) {

				magnetBlocks[i] = colision->magnetBlocks[i];

				magnetBlocks[i].Update();

			}

			player->Update(cameraState);

			goal->isGoal = player->GetIsGoal();

			goal->Update();

			//playUISprite->color.z = 0.0f;
			//playUISprite->Update();

			ImGui::Begin("block");
			ImGui::SliderFloat("rotaX", &rota.x, 0.0f, 10.0f);
			ImGui::SliderFloat("rotaY", &rota.y, 0.0f, 10.0f);
			ImGui::SliderFloat("rotaZ", &rota.z, 0.0f, 10.0f);
			if (ImGui::Button("rota reset")) {
				rota = { 0,0,0 };
			}
			ImGui::End();
			//マップの描画
			for (int i = 0; i < map_->blockY; i++)
			{
				for (int j = 0; j < map_->blockZ; j++)
				{
					for (int k = 0; k < map_->blockX; k++)
					{
						//	blockObj[i][j][k].rotation = rota;
						blockObj[i][j][k].Update();
					}
				}
			}

			//↓------------カメラ--------------↓


			if (input->IsKeyTrigger(DIK_UP) || input->IsTriggerRStickUp()) {

				if (cameraState == 0) {
					cameraState = 1;
				}
				else if (cameraState != 0) {
					cameraState = 0;
				}

				camera.ChangeState(cameraState);
			}
			else if (input->IsKeyTrigger(DIK_DOWN) || input->IsTriggerRStickDown()) {

				if (cameraState == 0) {
					cameraState = 2;
				}
				camera.ChangeState(cameraState);
			}
			else if (input->IsKeyTrigger(DIK_LEFT) || input->IsTriggerRStickLeft()) {


				if (cameraState == 0) {
					cameraState = 3;
				}
				else if (cameraState == 1) {
					cameraState = 4;
				}
				else if (cameraState == 2) {
					cameraState = 3;
				}
				else if (cameraState == 3) {
					cameraState = 1;
				}
				else if (cameraState == 4) {
					cameraState = 2;
				}
				camera.ChangeState(cameraState);
			}
			else if (input->IsKeyTrigger(DIK_RIGHT) || input->IsTriggerRStickRight()) {

				if (cameraState == 0) {
					cameraState = 4;
				}
				else if (cameraState == 1) {
					cameraState = 3;
				}
				else if (cameraState == 2) {
					cameraState = 4;
				}
				else if (cameraState == 3) {
					cameraState = 2;
				}
				else if (cameraState == 4) {
					cameraState = 1;
				}
				camera.ChangeState(cameraState);
			}

			camera.Update(player->GetPosition());

			//fps表示
			ImGui::Begin("fcamera");
			ImGui::Text("cameraState = %d", cameraState);
			ImGui::End();


			//↑------------カメラ--------------↑


		}
	}


	//----------------------ゲーム内ループはここまで---------------------//


}

void GamePlayScene::Draw()
{
	//-------背景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	backGroundSprite->Draw();

	//-------3Dオブジェクト描画処理-------//
	Object3d::BeginDraw(camera);

	//自機描画
	player->Draw();

	//磁石描画
	for (int i = 0; i < magnetBlocks.size(); i++) {
		magnetBlocks[i].Draw(magnetTextureN, magnetTextureS);
	}

	//マップの描画
	for (int i = 0; i < map_->blockY; i++)
	{


		for (int j = 0; j < map_->blockZ; j++)
		{
			for (int k = 0; k < map_->blockX; k++)
			{
				if (map_->map[i][j][k] == 1)
				{
					/*if (i != 1) {
						blockObj[i][j][k].model->textureIndex = groundTexture;
					}*/


					blockObj[i][j][k].Draw();
				}

			}
		}
	}

	goal->Draw();

	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	playUISprite->Draw();

	if (goal->isGoal) {
		clearSprite->Draw();

		/*clearNextSprite->color = { 0,0,0,1 };*/
		clearNextSprite->Update();
		clearNextSprite->Draw();
		clearStageSerectSprite->Draw();


	}

	if (isMenu) {
		menuSprite->Draw();
		/*selectBoxSprite->Draw();*/

		//menuResetSprite->color = {0,0,0,1};
		//menuTitleSprite->color = { 0,0,0,1 };
		//menuStageSerectSprite->color = { 0,0,0,1 };

		menuResetSprite->Update();
		menuTitleSprite->Update();
		menuStageSerectSprite->Update();


		menuResetSprite->Draw();
		menuTitleSprite->Draw();
		menuStageSerectSprite->Draw();
	}

	//シーン遷移用スプライト描画
	for (int i = 0; i < _countof(sceneChangeSprite); i++) {
		sceneChangeSprite[i]->Draw();
	}

}

void GamePlayScene::SetStage(int stageNumber)
{

	//Tutoattract,	//0
	//	Whichload,		//1
	//	Dontpanic,		//2
	//	Switching,		//3
	//	Down,			//4
	//	Order,			//5
	//	Tutorepulsion,	//6
	//	Direction,		//7
	//	Needmagnet,		//8
	//	Jam,			//9
	//	Mislead,		//10

	switch (stageNumber)
	{

	case Tutoattract:
		stageStr = "map/Tutoattract.csv";

		stageSize = { 20,3,20 };
		break;
	case Whichload:
		stageStr = "map/whichload.csv";
		stageSize = { 20,4,20 };
		break;
	case Dontpanic:
		stageStr = "map/dontpanic.csv";
		stageSize = { 20,3,20 };
		break;
	case Switching:
		stageStr = "map/switching.csv";
		stageSize = { 3,5,20 };
		break;
	case Down:
		stageStr = "map/down.csv";
		stageSize = { 20,4,20 };
		break;
	case Order:
		stageStr = "map/order.csv";
		stageSize = { 20,3,20 };
		break;
	case Tutorepulsion:
		stageStr = "map/Tutorepulsion.csv";
		stageSize = { 20,3,20 };
		break;
	case Direction:
		stageStr = "map/direction.csv";
		stageSize = { 20,4,20 };
		break;
	case Needmagnet:
		stageStr = "map/needmagnet.csv";
		stageSize = { 20,4,20 };
		break;
	case Jam:
		stageStr = "map/Jam.csv";
		stageSize = { 20,3,20 };
		break;
	case Mislead:
		stageStr = "map/mislead.csv";
		stageSize = { 20,5,20 };
		break;


	default:
		break;
	}

}

void GamePlayScene::StageInitialize(int stageNumber)
{
	//ステージ選択時に変更された文字列で読み込む
	SetStage(stageNumber);

	map_->SetSize(stageSize);
	map_->Loding(stageStr.c_str());

	//ゴールの初期化
	goal = new Goal;

	//プレイヤー初期化
	player = new Player();
	player->Initialize(playerTexture, playerTextureN, playerTextureS, input, map_, goal);

	//磁石関係初期化
	magnetDatas.clear();
	magnetBlocks.clear();

	//マップの座標の初期化
	for (int i = 0; i < map_->blockY; i++)
	{
		for (int j = 0; j < map_->blockZ; j++)
		{
			for (int k = 0; k < map_->blockX; k++)
			{
				blockObj[i][j][k].Initialize();
				//	blockObj[i][j][k].model = Model::CreateModel();
				//	blockObj[i][j][k].model->textureIndex = groundTextures[static_cast<int>(Random(0,4))];
				blockObj[i][j][k].position.x = k * blockSize * blockScale;
				blockObj[i][j][k].position.y = i * blockSize * blockScale;
				blockObj[i][j][k].position.z = j * blockSize * blockScale;
				//	blockObj[i][j][k].scale = { blockScale,blockScale,blockScale };
				blockObj[i][j][k].scale = { 0.5f,0.5f,0.5f };
				//blockObj[i][j][k].rotation.y =( XM_PI );
				blockObj[i][j][k].rotation.z = (XM_PI / 2.0f);
				blockObj[i][j][k].Update();

				if (map_->map[i][j][k] == 2) {
					MagnetData nBlockPos{ XMFLOAT3(k * blockSize * blockScale,i * blockSize * blockScale,j * blockSize * blockScale),true };
					magnetDatas.push_back(nBlockPos);
				}

				if (map_->map[i][j][k] == 3) {
					MagnetData sBlockPos{ XMFLOAT3(k * blockSize * blockScale,i * blockSize * blockScale,j * blockSize * blockScale), false };
					magnetDatas.push_back(sBlockPos);
				}

				if (map_->map[i][j][k] == 4)
				{
					player->SetPosition({ k * blockSize * blockScale,i * blockSize * blockScale,j * blockSize * blockScale });
					//player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
				}
				if (map_->map[i][j][k] == 5)
				{
					goal->Initialize(input, goalTexture, XMFLOAT3(k * blockSize * blockScale, i * blockSize * blockScale, j * blockSize * blockScale));
					goal->obj.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
					goal->obj.Update();
				}
			}
		}
	}

	//磁石の初期化と生成
	for (int i = 0; i < magnetDatas.size(); i++) {
		MagnetBlock newBlock{};
		newBlock.Initialize(magnetDatas[i]);
		//ゲームで使うようの配列に格納
		magnetBlocks.push_back(newBlock);
		//magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
		magnetBlocks[i].obj.rotation.x = (DirectX::XM_PI / 2.0f);
	}

	//当たり判定初期化
	colision = new Colision();
	for (int i = 0; i < magnetDatas.size(); i++) {
		colision->Initialize(player, magnetBlocks[i], map_, i);
	}

	//カメラにマップ情報を渡す
	camera.InitializeData(stageSize);

	//メニューは開かれていない状態
	isMenu = false;
}

void GamePlayScene::GoTitle()
{
}

void GamePlayScene::GoStageSelect()
{
}
