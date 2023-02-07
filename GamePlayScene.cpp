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

	//テクスチャデータ初期化
	magnetTextureN = Texture::LoadTexture(L"Resources/magnetN.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/magnetS.png");
	groundTexture = Texture::LoadTexture(L"Resources/testGround.png");
	groundTextures[0] = Texture::LoadTexture(L"Resources/groundPattern1.png");
	groundTextures[1] = Texture::LoadTexture(L"Resources/groundPattern2.png");
	groundTextures[2] = Texture::LoadTexture(L"Resources/groundPattern3.png");
	groundTextures[3] = Texture::LoadTexture(L"Resources/groundPattern4.png");
	playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");
	backGroundTexture = Texture::LoadTexture(L"Resources/playGameBack.png");
	clearTexture = Texture::LoadTexture(L"Resources/clear.png");
	goalTexture = Texture::LoadTexture(L"Resources/yellow1x1.png");
	menuTexture = Texture::LoadTexture(L"Resources/dummyIngameMenu.png");

	selectBoxTexture = Texture::LoadTexture(L"Resources/dummyUI_selectBox.png");
	selectBoxSprite = new Sprite();
	selectBoxSprite->Initialize(selectBoxTexture);
	selectBoxSprite->SetAnchorPoint({ 0.5f,0.5f });
	selectBoxSprite->Update();
	boxPos = selectBoxSprite->GetPosition();

	playUITexture = Texture::LoadTexture(L"Resources/dummyUI_PLAY.png");
	playUISprite = new Sprite();
	playUISprite->Initialize(playUITexture);
	playUISprite->SetAnchorPoint({ 0.0f,1.0f });
	playUISprite->SetPos({ 64.0f, WindowsAPI::winH - 64.0f });
	playUISprite->Update();

	backGroundSprite = new Sprite();
	backGroundSprite->Initialize(backGroundTexture);

	menuSprite = new Sprite();
	menuSprite->Initialize(menuTexture);
	//アンカーポイントをスプライトの中心に
	menuSprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	menuSprite->SetPos(XMFLOAT2(WindowsAPI::winW / 2, WindowsAPI::winH / 2));
	menuSprite->Update();

	goalSprite.Initialize(clearTexture);
	goalSprite.SetPos(XMFLOAT2(400, 200));
	//goalSprite.SetSize(XMFLOAT2(100, 100));
	goalSprite.Update();

	selectBoxPos[0] = { WindowsAPI::winW / 2,320.0f };
	selectBoxPos[1] = { WindowsAPI::winW / 2,400.0f };
	selectBoxPos[2] = { WindowsAPI::winW / 2,480.0f };

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
	//-------------ここまでにループ内で使用したものの後処理------------//
}

void GamePlayScene::Update()
{

	//----------------------ゲーム内ループはここから---------------------//

	if (goal->isGoal) {
		//スティック左右でメニューを選ぶ
		if (input->IsTriggerLStickRight() || input->IsKeyTrigger(DIK_D)) {
			clearMenuNumber++;
		}
		else if (input->IsTriggerLStickLeft() || input->IsKeyTrigger(DIK_A)) {
			clearMenuNumber--;
		}

		//最大値、最小値を超えないように
		if (clearMenuNumber > 1)clearMenuNumber = 1;
		else if (clearMenuNumber < 0)clearMenuNumber = 0;

		//Aボタンで決定
		if (input->IsPadTrigger(XINPUT_GAMEPAD_A) || input->IsKeyTrigger(DIK_SPACE)) {
			//メニュー選択番号が0かつ、最終ステージでないならなら次のステージへ
			if (clearMenuNumber == 0 && ShareData::stageNumber < StageIndex::tutorial1) {
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
					sceneManager->ChangeScene("TITLE");
				}
				else if (selectMenuNumber == Title) {

					//共通データのフェーズを入力待ち(タイトル画面)に変更し、タイトルシーンへ戻る
					ShareData::titlePhase = TitlePhaseIndex::WaitInputSpaceKey;
					sceneManager->ChangeScene("TITLE");
				}

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

			selectBoxSprite->SetPos(selectBoxPos[selectMenuNumber]);
			selectBoxSprite->Update();

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

			player->Update();

			goal->isGoal = player->GetIsGoal();

			goal->Update();

			//↓------------カメラ--------------↓


			if (input->IsKeyTrigger(DIK_RETURN)) {
				cameraState++;
				if (cameraState >= 5) {
					cameraState = 0;
				}
				camera.ChangeState(cameraState);
			}

			if (input->IsKeyTrigger(DIK_UP)) {

				if (cameraState == 0) {
					cameraState = 1;
				}
				else if (cameraState != 0) {
					cameraState = 0;
				}				

				camera.ChangeState(cameraState);
			}
			else if (input->IsKeyTrigger(DIK_DOWN)) {

				if (cameraState == 0) {
					cameraState = 2;
				}
				camera.ChangeState(cameraState);
			}
			else if (input->IsKeyTrigger(DIK_LEFT)) {

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
			else if (input->IsKeyTrigger(DIK_RIGHT)) {

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
					if (i != 1) {
						blockObj[i][j][k].model->textureIndex = groundTexture;
					}


					blockObj[i][j][k].Draw();
				}
			}
		}
	}

	goal->Draw();

	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();

	//playUISprite->Draw();

	if (goal->isGoal) {
		goalSprite.Draw();
	}

	if (isMenu) {
		menuSprite->Draw();
		selectBoxSprite->Draw();
	}


}

void GamePlayScene::SetStage(int stageNumber)
{
	switch (stageNumber)
	{
	case Sample1:
		stageStr = "map/map1.csv";
		stageSize = { 10,5,10 };
		break;
	case Sample2:
		stageStr = "map/order.csv";
		stageSize = { 20,3,20 };
		break;
	case Sample3:
		stageStr = "map/whichload.csv";
		stageSize = { 20,4,20 };
		break;
	case Sample4:
		stageStr = "map/dontpanic.csv";
		stageSize = { 20,3,20 };
		break;
	case tutorial1:
		stageStr = "map/Tuto1.csv";
		stageSize = { 20,3,20 };
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
	player->Initialize(playerTexture, magnetTextureN, magnetTextureS, input, map_, goal);

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
				blockObj[i][j][k].model = Model::CreateModel();
				blockObj[i][j][k].model->textureIndex = groundTextures[static_cast<int>(Random(0,4))];
				blockObj[i][j][k].position.x = k * blockSize * blockScale;
				blockObj[i][j][k].position.y = i * blockSize * blockScale;
				blockObj[i][j][k].position.z = j * blockSize * blockScale;
				blockObj[i][j][k].scale = { blockScale,blockScale,blockScale };
				blockObj[i][j][k].rotation.x = XM_PI / 2.0f;
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
