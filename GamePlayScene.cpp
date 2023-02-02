#include "GamePlayScene.h"
#include"Texture.h"
#include"DirectX.h"
#include"SpriteManager.h"
#include"GameSceneManager.h"
#include"ShareData.h"

using namespace DirectX;

void GamePlayScene::Initialize()
{


	//--------------ゲーム内変数初期化--------------//

	//inputのインスタンス取得
	input = Input::GetInstance();

	//テクスチャデータ初期化
	magnetTextureN = Texture::LoadTexture(L"Resources/red1x1.png");
	magnetTextureS = Texture::LoadTexture(L"Resources/blue1x1.png");
	groundTexture = Texture::LoadTexture(L"Resources/ground.png");
	playerTexture = Texture::LoadTexture(L"Resources/white1x1.png");
	backGroundTexture = Texture::LoadTexture(L"Resources/dummyPlayGame.png");
	clearTexture = Texture::LoadTexture(L"Resources/clear.png");
	goalTexture = Texture::LoadTexture(L"Resources/yellow1x1.png");
	menuTexture = Texture::LoadTexture(L"Resources/dummyIngameMenu.png");

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
	//-------------ここまでにループ内で使用したものの後処理------------//
}

void GamePlayScene::Update()
{

	//----------------------ゲーム内ループはここから---------------------//

	if (goal->isGoal) {
		//スティック左右でメニューを選ぶ
		if (input->IsTriggerLStickRight()) {
			clearMenuNumber++;
		}
		else if (input->IsTriggerLStickLeft()) {
			clearMenuNumber--;
		}

		//最大値、最小値を超えないように
		if (clearMenuNumber >1)clearMenuNumber = 1;
		else if (clearMenuNumber < 0)clearMenuNumber = 0;

		//Aボタンで決定
		if (input->IsPadTrigger(XINPUT_GAMEPAD_A)) {
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
			if (input->IsPadTrigger(XINPUT_GAMEPAD_START)) {
				isMenu = false;
			}

			//スティック上下でメニューを選ぶ
			if (input->IsTriggerLStickDown()) {
				selectMenuNumber++;
			}
			else if (input->IsTriggerLStickUp()) {
				selectMenuNumber--;
			}

			//最大値、最小値を超えないように
			if (selectMenuNumber > MenuIndex::Title)selectMenuNumber = Title;
			else if (selectMenuNumber < MenuIndex::Reset)selectMenuNumber = Reset;

			if (input->IsPadTrigger(XINPUT_GAMEPAD_A)) {

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
			ImGui::End();

		}
		else {

			//スタートボタンでメニューへ
			if (input->IsPadTrigger(XINPUT_GAMEPAD_START)) {
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

			//カメラ座標は自機に追従
			camera.target.x = player->GetPosition().x;
			camera.target.y = player->GetPosition().y;
			camera.target.z = player->GetPosition().z;
			camera.eye = camera.target;
			camera.eye.y += 20.0f;
			camera.eye.z -= 2.5f;

			camera.UpdateMatrix();

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
					blockObj[i][j][k].Draw();
				}
			}
		}
	}

	goal->Draw();

	//-------前景スプライト描画処理-------//
	SpriteManager::GetInstance()->beginDraw();


	if (goal->isGoal) {
		goalSprite.Draw();
	}

	if (isMenu) {
		menuSprite->Draw();
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
		stageStr = "map/map2.csv";
		stageSize = { 10,3,10 };
		break;
	case Sample3:
		stageStr = "map/map3.csv";
		stageSize = { 20,4,20 };
		break;
	case Sample4:
		stageStr = "map/map4.csv";
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
				blockObj[i][j][k].model->textureIndex = groundTexture;
				blockObj[i][j][k].position.x = k * blockSize * blockScale;
				blockObj[i][j][k].position.y = i * blockSize * blockScale;
				blockObj[i][j][k].position.z = j * blockSize * blockScale;
				blockObj[i][j][k].scale = { blockScale,blockScale,blockScale };
				blockObj[i][j][k].Update();

				if (map_->map[i][j][k] == 2) {
					MagnetData nBlockPos{ XMFLOAT3(k * blockSize * blockScale,i * blockSize * blockScale + 1,j * blockSize * blockScale),true };
					magnetDatas.push_back(nBlockPos);
				}

				if (map_->map[i][j][k] == 3) {
					MagnetData sBlockPos{ XMFLOAT3(k * blockSize * blockScale,i * blockSize * blockScale + 1,j * blockSize * blockScale), false };
					magnetDatas.push_back(sBlockPos);
				}

				if (map_->map[i][j][k] == 4)
				{
					player->SetPosition({ k * blockSize * blockScale,i * blockSize * blockScale,j * blockSize * blockScale });
					player->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
				}
				if (map_->map[i][j][k] == 5)
				{
					goal->Initialize(input, goalTexture, XMFLOAT3(k * blockSize * blockScale, i * blockSize * blockScale, j * blockSize * blockScale));
					goal->obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
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
		magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	}

	//当たり判定初期化
	colision = new Colision();
	for (int i = 0; i < magnetDatas.size(); i++) {
		colision->Initialize(player, magnetBlocks[i], map_, i);
	}


	//メニューは開かれていない状態
	isMenu = false;
}

void GamePlayScene::GoTitle()
{
}

void GamePlayScene::GoStageSelect()
{
}
