
#include "Player.h"
#include"ImguiManager.h"
using namespace DirectX;

void Player::Initialize(const uint32_t& texW, const uint32_t& texR, const uint32_t& texB, Input* input, Map* map, Goal* goal)
{
	//引数からメンバへ
	whiteTexture = texW;
	blueTexture = texB;
	redTexture = texR;
	this->input = input;

	//オブジェクトデータ初期化
	obj.Initialize();
	obj.model = Model::CreateModel();
	//プレイヤーのデフォルトカラーは白
	playerTexture = whiteTexture;
	//サイズ調整
	obj.scale = XMFLOAT3(size / 10, size / 10, size / 10);



	this->map = map;
	this->goal = goal;
}

void Player::Update()
{
	//移動
	Move();

	Jump();

	//落下
	Fall();

	//自機の状態を変える
	ChangeState();

	//自機の状態によって使う画像を変える
	if (state == UnMagnet) {
		playerTexture = whiteTexture;
	}
	else if (state == NorthPole) {
		playerTexture = redTexture;
	}
	else if (state == SouthPole) {
		playerTexture = blueTexture;
	}

	//ゴール判定
	GoalCol();

	//行列更新
	obj.Update();

	ImGui::Begin("jump");
	ImGui::Text("%d",isJump);
	ImGui::End();

	ImGui::Begin("fall");
	ImGui::Text("%d",fall);
	ImGui::End();

}

void Player::Draw()
{
	obj.model->textureIndex = playerTexture;
	obj.Draw();
}

void Player::Fall()
{


	if (fall && isJump == false && colY.y == 0) {

		pos.y -= playerSpd;
		move.y = -playerSpd;

	}


	obj.position = pos;

}

void Player::Jump()
{
	//if (input->IsKeyPress(DIK_RETURN)) {
	//	isJump = true;
	//}

	if (isJump) {

		pos.y += playerSpd;
		move.y = playerSpd;

		obj.position = pos;

		if (jumpBeforePosY + jumpHight <= pos.y) {
			isJump = false;
			fall = true;
		}

	}
	else {
		jumpBeforePosY = pos.y;
	}

}

void Player::GoalCol()
{

	float pPosX1 = pos.x - (size / 2);
	float pPosX2 = pos.x + (size / 2);

	float pPosY1 = pos.y - (size / 2);
	float pPosY2 = pos.y + (size / 2);

	float pPosZ1 = pos.z - (size / 2);
	float pPosZ2 = pos.z + (size / 2);


	float gPosX1 = goal->GetPos().x - (size / 2);
	float gPosX2 = goal->GetPos().x + (size / 2);

	float gPosY1 = goal->GetPos().y - (size / 2);
	float gPosY2 = goal->GetPos().y + (size / 2);

	float gPosZ1 = goal->GetPos().z - (size / 2);
	float gPosZ2 = goal->GetPos().z + (size / 2);

	float adjust = 0.0;

	if (pPosX1 < gPosX2 + adjust && gPosX1 - adjust < pPosX2) {

		if (pPosY1 < gPosY2 && gPosY1 < pPosY2) {

			if (pPosZ1 < gPosZ2 + adjust && gPosZ1 - adjust < pPosZ2) {

				//あったったらあったった時の処理
				goal->InGoal();

			}
		}
	}

}

void Player::OnMapCollision()
{
}

void Player::OnMapCollisionX()
{
	pos.x -= adjustPixelSpeed;
}

void Player::OnMapCollisionY()
{
	pos.y -= adjustPixelSpeed;
}

void Player::OnMapCollisionZ()
{
	pos.z -= adjustPixelSpeed;
}

void Player::OnMapCollisionX2()
{
	pos.x += adjustPixelSpeed;
}

void Player::OnMapCollisionY2()
{
	pos.y += adjustPixelSpeed;
}

void Player::OnMapCollisionZ2()
{
	pos.z += adjustPixelSpeed;
}

void Player::ChangeState()
{
	if (input->IsPadTrigger(XINPUT_GAMEPAD_A) || input->IsKeyTrigger(DIK_SPACE)) {
		state++;
		//state = SouthPole;
		if (state > SouthPole) {
			state = UnMagnet;
		}
	}
}

void Player::Move() {

	if (input->IsDownLStickLeft() || input->IsKeyPress(DIK_A)) {
		if (colX.y == 0) {
			pos.x -= playerSpd;
			move.x = -playerSpd;
		}
	}
	else  if (input->IsDownLStickRight() || input->IsKeyPress(DIK_D)) {
		if (colX.x == 0) {
			pos.x += playerSpd;
			move.x = playerSpd;
		}
	}
	else {
		move.x = 0;
	}

	if (input->IsDownLStickDown() || input->IsKeyPress(DIK_S)) {
		if (colZ.y == 0) {

			pos.z -= playerSpd;
			move.z = -playerSpd;
		}
	}
	else  if (input->IsDownLStickUp() || input->IsKeyPress(DIK_W)) {
		if (colZ.x == 0) {

			pos.z += playerSpd;
			move.z = playerSpd;
		}
	}
	else {
		move.z = 0;
	}



	//自機座標をimguiでいじる
	//ImGui::Begin("player");
	//ImGui::SliderFloat("pos.x", &pos.x, -10.0f, 10.0f);
	//ImGui::SliderFloat("pos.z", &pos.z, -10.0f, 10.0f);
	//ImGui::Text("imgui iikanji!!!");
	//ImGui::End();

	obj.position = pos;
}




