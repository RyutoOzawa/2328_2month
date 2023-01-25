
#include "Player.h"
using namespace DirectX;

void Player::Initialize(const uint32_t& texW, const uint32_t& texR, const uint32_t& texB, Input* input)
{
	//�������烁���o��
	whiteTexture = texW;
	blueTexture = texB;
	redTexture = texR;
	this->input = input;

	//�I�u�W�F�N�g�f�[�^������
	obj.Initialize();
	obj.model = Model::CreateModel();
	//�v���C���[�̃f�t�H���g�J���[�͔�
	playerTexture = whiteTexture;
	//�T�C�Y����
	obj.scale = XMFLOAT3(0.99f, 0.99f, 0.99f);
}

void Player::Update()
{
	//�ړ�
	Move();

	//���@�̏�Ԃ�ς���
	ChangeState();

	//���@�̏�Ԃɂ���Ďg���摜��ς���
	if (state == UnMagnet) {
		playerTexture = whiteTexture;
	}
	else if (state == NorthPole) {
		playerTexture = redTexture;
	}
	else if (state == SouthPole) {
		playerTexture = blueTexture;
	}

	//�s��X�V
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




