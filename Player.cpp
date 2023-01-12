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
	//プレイヤーのデフォルトカラーは白
	playerTexture = whiteTexture;
//サイズ調整
	obj.scale = XMFLOAT3(0.99f, 0.99f, 0.99f);
}

void Player::Update()
{
}

void Player::Draw()
{
}

void Player::OnMapCollision()
{
}

void Player::OnMapCollisionX()
{
}

void Player::OnMapCollisionY()
{
}

void Player::OnMapCollisionZ()
{
}

void Player::OnMapCollisionX2()
{
}

void Player::OnMapCollisionY2()
{
}

void Player::OnMapCollisionZ2()
{
}
