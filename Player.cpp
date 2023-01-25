
#include "Player.h"
using namespace DirectX;

void Player::Initialize(const uint32_t& texW, const uint32_t& texR, const uint32_t& texB, Input* input)
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
	obj.scale = XMFLOAT3(0.99f, 0.99f, 0.99f);
}

void Player::Update()
{
	//移動
	Move();

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

	//行列更新
	obj.Update();
}

void Player::Draw()
{
	obj.model->textureIndex = playerTexture;
	obj.Draw();
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
	if (input->IsTrigger(DIK_SPACE)) {
		state++;
		//state = SouthPole;
		if (state > SouthPole) {
			state = UnMagnet;
		}
	}
}

void Player::Move() {

	if(input->IsPress(DIK_A)) {
		if (colX.y == 0) {
			pos.x -= playerSpd;
			move.x = -playerSpd;
		}
	}
	else  if (input->IsPress(DIK_D)) {
		if (colX.x == 0) {
			pos.x += playerSpd;
			move.x = playerSpd;
		}
	}
	else {
		move.x = 0;
	}

	if (input->IsPress(DIK_S)) {
		if (colX.y == 0) {
			pos.z -= playerSpd;
			move.z = -playerSpd;
		}
	}
	else  if (input->IsPress(DIK_W)) {
		if (colX.x == 0) {
			pos.z += playerSpd;
			move.z = playerSpd;
		}
	}
	else {
		move.z = 0;
	}

	obj.position = pos;
}




