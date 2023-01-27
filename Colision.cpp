#include "Colision.h"
#include <cassert>
#include"ImguiManager.h"

Colision::Colision() {}

Colision::~Colision() {
}

void Colision::Initialize(Player* player, MagnetBlock magnetBlock, int i) {

	//this->player = player;

	//magnetBlocks[i] = magnetBlock;

	//player = new Player();


	//�Q�[���Ŏg���悤�̔z��Ɋi�[
	magnetBlocks.push_back(magnetBlock);
	magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);

}

void Colision::Update() {

	//���͐؂�ւ�
	MagnetPower();

	//���W�֌W�X�V
	InforUpdate();

	//���΂̈ړ�
	MagnetsUpdate();

	//�u���b�N�Ƃ̓����蔻��
	MapCollision();

	//���΂Ǝ��@�̓����蔻��(�����߂�����)
	PosCollision();

	//�������Ă�u���b�N�̃x�N�g���𓝈�
	StickMag();

	//���W�̍X�V
	for (int i = 0; i < magnetBlocks.size(); i++) {
		//�v�Z�������W���m��

		setPos[i].x += bMoveVec[i].x;
		setPos[i].y += bMoveVec[i].y;
		setPos[i].z += bMoveVec[i].z;

		//ImGui::Begin("setPos");
		//ImGui::Text("%d = %f,%f,%f \n", i, setPos[i].x, setPos[i].y, setPos[i].z);
		//ImGui::End();

		magnetBlocks[i].SetPos(setPos[i]);



		//magnetBlocks[i].Update();

		//debugText_->SetPos(0, 0);
		//debugText_->Printf("setPos[0] = (%f,%f,%f)", setPos[0].x, setPos[0].y, setPos[0].z);

	}

}

void Colision::UpdateDeta(Player* player, MagnetBlock magnetBlock, int i)
{

	this->player = player;

	magnetBlocks[i] = magnetBlock;

}

//����
void Colision::MapCollision()
{

	//������Ȃ��悤��������p
	float adjustPixcelSpeed = player->GetAdjustPixelSpeed();
	//���@�T�C�Y�����p
	float adjustPlayerSize = 0.03;

	//���W��p��
	float leftplayer = player->GetPosition().x + adjustPlayerSize;
	float downplayer = player->GetPosition().y + adjustPlayerSize;
	float frontplayer = player->GetPosition().z + adjustPlayerSize;

	float rightplayer = player->GetPosition().x + player->GetSize() - adjustPlayerSize;
	float upplayer = player->GetPosition().y - player->GetSize() - adjustPlayerSize;
	float backplayer = player->GetPosition().z + player->GetSize() - adjustPlayerSize;

	//�������Ă��邩
	//Vector2 ColX = { 0,0 };
	//Vector2 ColY = { 0,0 };
	//Vector2 ColZ = { 0,0 };

	float playerSpeed = player->GetSpeed() + adjustPixcelSpeed;

	/////////////
	//�v���C���[//
	/////////////


	//�E�ɉ��z�I�Ɉړ����ē���������
	if (savemap_->mapcol(rightplayer + playerSpeed, downplayer + player->GetSize() / 2, frontplayer) || savemap_->mapcol(rightplayer + playerSpeed, downplayer + player->GetSize() / 2, backplayer))
	{

		if (player->GetMove().x > 0 && ColX.x == 0) {
			//�P�s�N�Z����ɕǂ�����܂ňړ�
			while (true)
			{
				if ((savemap_->mapcol(rightplayer + adjustPixcelSpeed, downplayer + player->GetSize() / 2, frontplayer) || savemap_->mapcol(rightplayer + adjustPixcelSpeed, downplayer + player->GetSize() / 2, backplayer))) {
					break;
				}

				player->OnMapCollisionX2();
				rightplayer = player->GetPosition().x + player->GetSize() - adjustPlayerSize;
				leftplayer = player->GetPosition().x + adjustPlayerSize;

			}

			ColX.x = 1;
		}

	}
	else {
		ColX.x = 0;
	}

	//debugText_->SetPos(0,0);
	//debugText_->Printf("RIGHT = %f",ColX.x);

	//���ɉ��z�I�Ɉړ����ē���������
	if (savemap_->mapcol(leftplayer - playerSpeed, downplayer + player->GetSize() / 2, frontplayer) || savemap_->mapcol(leftplayer - playerSpeed, downplayer + player->GetSize() / 2, backplayer))
	{
		if (player->GetMove().x < 0 && ColX.y == 0) {
			//�P�s�N�Z����ɕǂ�����܂ňړ�
			while (true)
			{
				if ((savemap_->mapcol(leftplayer - adjustPixcelSpeed, downplayer + player->GetSize() / 2, frontplayer) || savemap_->mapcol(leftplayer - adjustPixcelSpeed, downplayer + player->GetSize() / 2, backplayer))) {
					break;
				}

				player->OnMapCollisionX();
				rightplayer = player->GetPosition().x + player->GetSize() - adjustPlayerSize;
				leftplayer = player->GetPosition().x + adjustPlayerSize;
			}

			ColX.y = 1;

		}

	}
	else {
		ColX.y = 0;
	}


	//debugText_->SetPos(0, 20);
	//debugText_->Printf("LEFT  = %f", ColX.y);

	//leftplayer = player->GetPosition().x + adjust;
	//rightplayer = player->GetPosition().x + player->GetSize() - adjust;

	//upplayer = player->GetPosition().y - player->GetSize() - adjust;
	//downplayer = player->GetPosition().y + adjust;


	//z���ɑ΂��Ă̓����蔻��
	//���ɉ��z�I�Ɉړ����ē���������
	if (savemap_->mapcol(leftplayer, downplayer + player->GetSize() / 2, backplayer + playerSpeed) || savemap_->mapcol(rightplayer, downplayer + player->GetSize() / 2, backplayer + playerSpeed))
	{
		if (player->GetMove().z > 0 && ColZ.x == 0) {
			//�P�s�N�Z����ɕǂ�����܂ňړ�
			while (true)
			{
				if ((savemap_->mapcol(leftplayer, downplayer + player->GetSize() / 2, backplayer + adjustPixcelSpeed) || savemap_->mapcol(rightplayer, downplayer + player->GetSize() / 2, backplayer + adjustPixcelSpeed))) {
					break;
				}

				player->OnMapCollisionZ2();
				frontplayer = player->GetPosition().z + adjustPlayerSize;
				backplayer = player->GetPosition().z + player->GetSize() - adjustPlayerSize;
			}

			ColZ.x = 1;

		}
	}
	else {
		ColZ.x = 0;
	}


	//debugText_->SetPos(0, 40);
	//debugText_->Printf("UP    = %f", ColZ.x);

	//��O�ɉ��z�I�Ɉړ����ē���������
	if (savemap_->mapcol(leftplayer, downplayer + player->GetSize() / 2, frontplayer - playerSpeed) || savemap_->mapcol(rightplayer, downplayer + player->GetSize() / 2, frontplayer - playerSpeed))
	{
		if (player->GetMove().z < 0 && ColZ.y == 0) {
			//�P�s�N�Z����ɕǂ�����܂ňړ�
			while (true)
			{
				if ((savemap_->mapcol(leftplayer, downplayer + player->GetSize() / 2, frontplayer - adjustPixcelSpeed) || savemap_->mapcol(rightplayer, downplayer + player->GetSize() / 2, frontplayer - adjustPixcelSpeed))) {
					break;
				}

				player->OnMapCollisionZ();
				frontplayer = player->GetPosition().z + adjustPlayerSize;
				backplayer = player->GetPosition().z + player->GetSize() - adjustPlayerSize;
			}

			ColZ.y = 1;

		}
	}
	else {
		ColZ.y = 0;
	}


	//debugText_->SetPos(0, 60);
	//debugText_->Printf("DOWN  = %f", ColZ.y);

	//if (ColX.x == 1) {

	//	if (ColZ.x == 1 && ColZ.y == 1) {
	//		ColX.x = 0;
	//	}

	//}

	//if (ColX.y == 1) {

	//	if (ColZ.x == 1) {
	//		ColX.y = 0;
	//	}

	//	if (ColZ.y == 1) {
	//		ColX.y = 0;
	//	}

	//}

	player->SetColX(ColX);
	player->SetColY(ColY);
	player->SetColZ(ColZ);

}

void Colision::PosCollision()
{

	ImGui::Begin("player");
	ImGui::Text("player = %f,%f,%f \n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	//ImGui::Text("pPos = %f \n", pSize);
	ImGui::End();

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//�u���b�N ���W

		bPos[i] = setPos[i];

		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;



		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;


	}

	//���@�Ǝ���

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

			if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

				XMFLOAT3 pos1 = pPos;
				XMFLOAT3 pos2 = { setPos[i].x + bMoveVec[i].x,setPos[i].y + bMoveVec[i].y ,setPos[i].z + bMoveVec[i].z };

				float pos1Size = pSize;
				float pos2Size = bSize;

				//�����p
				XMFLOAT3 adjust = GetVec(pos1, pos2);

				adjust = ChangeVec(adjust, 0.0001f);

				int contact = GetContact(pos1, pos2);

				if (pState != UnMagnet) {

					//�����߂�����
					while (true)
					{
						if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

							//�܂��d�Ȃ��Ă����痣��

							//���������̂�x����z�����ŉ����߂���ύX
							if (contact == 1 || contact == 2) {
								pos2.y += adjust.y;
								pos2.z += adjust.z;

								//pos1.y -= adjust.y;
								//pos1.z -= adjust.z;

								bMoveVec[i].y += adjust.y;
								bMoveVec[i].z += adjust.z;

							}
							else {
								pos2.x += adjust.x;
								pos2.y += adjust.y;

								//pos1.x -= adjust.x;
								//pos1.y -= adjust.y;


								bMoveVec[i].x += adjust.x;
								bMoveVec[i].y += adjust.y;

							}
						}
						else {
							break;
						}

					}

					//�������ĂȂ��Ƃ���ŃZ�b�g
					//player->SetPos(pos1);
					//setPos[i] = pos2;

					//�v���C���[�ړ��������Z
					bMoveVec[i].x += pMoveVec.x;
					bMoveVec[i].z += pMoveVec.z;

					//�����������ʂ��L�^
					if (pState == NorthPole && magnetBlocks[i].GetIsNorth() == 0) {
						magnetBlocks[i].SetContactNum(contact, i);
					}
					else if (pState == SouthPole && magnetBlocks[i].GetIsNorth() == 1) {
						magnetBlocks[i].SetContactNum(contact, i);
					}
					else {
						magnetBlocks[i].ReSetContactNum(contact);
					}

				}
				else {


					////�����߂�����
					//while (true)
					//{
					//	if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

					//		//�܂��d�Ȃ��Ă����痣��

					//		//���������̂�x����z�����ŉ����߂���ύX
					//		if (contact == 1 || contact == 2) {
					//			pos1.y -= adjust.y;
					//			pos1.z -= adjust.z;
					//		}
					//		else {
					//			pos1.x -= adjust.x;
					//			pos1.y -= adjust.y;
					//		}
					//	}
					//	else {
					//		break;
					//	}

					//}

					//�������ĂȂ��Ƃ���ŃZ�b�g
					//player->SetPos(pos1);
					// 
					//���@�����͂Ȃ��̏ꍇ�߂荞�܂��Ȃ��悤��
					if (contact == 1) {
						ColZ.y = 1;
					}
					else if (contact == 2) {
						ColZ.x = 1;
					}
					else if (contact == 3) {
						ColX.x = 1;
					}
					else if (contact == 4) {
						ColX.y = 1;
					}

					magnetBlocks[i].ReSetContactNum(contact);

					//debugText_->SetPos(120, 0);
					//debugText_->Printf("true");
				}

				//debugText_->SetPos(0, 0);
				//debugText_->Printf("p b[%d] true", i);

			}

		}

	}

	//----------���΃u���b�N�Ǝ��΃u���b�N----------

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//�u���b�N ���W


		bPos[i] = setPos[i];


		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;



		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;

	}

	//���������������ǂ������f����悤�̔z��

	//�����������Ƃ��̏��Ȃǂ��擾����

	for (int i = 0; i < magnetBlocks.size(); i++) {

		for (int j = 0; j < magnetBlocks.size(); j++) {

			//�������΂Ƃ������������g�ݍ��킹�͓����������������Ȃ��悤��

			//�������΂�

			if (j >= i) {
				break;
			}

			XMFLOAT3 pos1 = { setPos[i].x + bMoveVec[i].x, setPos[i].y + bMoveVec[i].y ,setPos[i].z + bMoveVec[i].z };

			XMFLOAT3 pos2 = { setPos[j].x + bMoveVec[j].x, setPos[j].y + bMoveVec[j].y ,setPos[j].z + bMoveVec[j].z };

			float pos1Size = bSize;
			float pos2Size = bSize;

			//�����p
			XMFLOAT3 adjust = GetVec(pos1, pos2);

			adjust = ChangeVec(adjust, 0.0001f);

			int contact1 = GetContact(pos2, pos1);
			int contact2 = GetContact(pos1, pos2);


			if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

				if (bPosZ1[j] < bPosZ2[i] && bPosZ1[i] < bPosZ2[j]) {

					//XMFLOAT3 pos1 = magnetBlocks[i].GetPos();
					//XMFLOAT3 pos2 = magnetBlocks[j].GetPos();

					//�����߂�����
					while (true)
					{
						if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

							//���������̂�x����z�����ŉ����߂���ύX
							if (contact1 == 1 || contact1 == 2) {

								pos1.y -= adjust.y;
								pos1.z -= adjust.z;

								pos2.y += adjust.y;
								pos2.z += adjust.z;

								bMoveVec[i].y -= adjust.y;
								bMoveVec[i].z -= adjust.z;

								bMoveVec[j].y += adjust.y;
								bMoveVec[j].z += adjust.z;

							}
							else {

								pos1.x -= adjust.x;
								pos1.y -= adjust.y;

								pos2.x += adjust.x;
								pos2.y += adjust.y;

								bMoveVec[i].x -= adjust.x;
								bMoveVec[i].y -= adjust.y;


								bMoveVec[j].x += adjust.x;
								bMoveVec[j].y += adjust.y;


							}

						}
						else {
							break;
						}

					}

					magnetBlocks[i].SetContactNum(contact1, j);
					magnetBlocks[j].SetContactNum(contact2, i);


					//�������ĂȂ��Ƃ���ŃZ�b�g
					//magnetBlocks[i].SetPos(pos1);
					//magnetBlocks[j].SetPos(pos2);
					//setPos[i] = pos1;
					//setPos[j] = pos2;

					//debugText_->SetPos(0, 20);
					//debugText_->Printf("b[%d] b[%d] true", i, j);
				}

			}

		}
	}


	//���@�Ǝ��΍��W�ύX�ňړ��������ύX

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//�u���b�N ���W

		bPos[i] = setPos[i];

		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;

		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;

	}

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

			if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

				XMFLOAT3 pos1 = pPos;
				XMFLOAT3 pos2 = { setPos[i].x + bMoveVec[i].x,setPos[i].y + bMoveVec[i].y ,setPos[i].z + bMoveVec[i].z };

				int contact = GetContact(pos1, pos2);

				if (pState != UnMagnet) {

					////�����������ʂ��L�^
					//if (pState == NorthPole && magnetBlocks[i].GetIsNorth() == 0) {
					//	magnetBlocks[i].SetContactNum(contact, i);
					//}
					//else if (pState == SouthPole && magnetBlocks[i].GetIsNorth() == 1) {
					//	magnetBlocks[i].SetContactNum(contact, i);
					//}
					//else {
					//	magnetBlocks[i].ReSetContactNum(contact);
					//}

				}
				else {


					ImGui::Begin("contact");
					ImGui::Text("contact[%d] = %d \n", i, contact);
					ImGui::End();


					//���@�����͂Ȃ��̏ꍇ�߂荞�܂��Ȃ��悤��
					if (contact == 1) {
						ColZ.y = 1;
					}
					else if (contact == 2) {
						ColZ.x = 1;
					}
					else if (contact == 3) {
						ColX.x = 1;
					}
					else if (contact == 4) {
						ColX.y = 1;
					}

					//magnetBlocks[i].ReSetContactNum(contact);
				}

			}

		}

	}

	player->SetColX(ColX);
	player->SetColY(ColY);
	player->SetColZ(ColZ);

}

void Colision::MagnetsUpdate() {

	MagToPlayerUpdate();

	MagToMagUpdate();


}

void Colision::MagToMagUpdate()
{

	//�z��̍ő吔-1��for������
	for (int i = 0; i < magnetBlocks.size() - 1; i++) {

		//i�ڂ̎��΂ɑ΂��āAi+1 ~ �z�񖖔��܂ł̃u���b�N�Ǝ��΂̔�����s��
		for (int j = i + 1; j < magnetBlocks.size(); j++) {

			// �����񂹏������ǂ����ON�̏ꍇ�����񂹂�悤��
			bool isMagMove = magnetBlocks[i].GetIsMagMove(j);

			if (isMagMove) {
				isMagMove = magnetBlocks[j].GetIsMagMove(i);
			}


			if (isMagMove) {

				bool isSame = false;
				//���ɂ��΋ɂ�
				if (magnetBlocks[i].GetIsNorth() == magnetBlocks[j].GetIsNorth()) {
					isSame = true;
				}

				//�u���b�N���m�̃x�N�g���쐬
				XMFLOAT3 vecMagToMag;
				vecMagToMag.x = (magnetBlocks[i].GetPos().x) - (magnetBlocks[j].GetPos().x);
				vecMagToMag.y = (magnetBlocks[i].GetPos().y) - (magnetBlocks[j].GetPos().y);
				vecMagToMag.z = (magnetBlocks[i].GetPos().z) - (magnetBlocks[j].GetPos().z);

				//�x�N�g���̒����擾
				float vecLen = lengthVec(vecMagToMag);

				float moveSpd = 0.025;

				//�����Ŏ��͂̋�����ω�������
				if (2.0f > vecLen) {
					moveSpd = ((2.0f / 1000) - (vecLen / 1000)) / 0.01;
				}



				//i�ڂ̎��΂�j�ڂ̎��͂ɂ�鋓��
				if (isSame) {
					if (vecLen <= 2.0f) {



						//�x�N�g���𐳋K��+���΂̑����ɒ���
						vecMagToMag = ChangeVec(vecMagToMag, moveSpd);
						//vecMagToMag *= moveSpd;
						//���ꂼ��̃u���b�N�ɉ��Z
						XMFLOAT3 pos1, pos2;
						pos1 = magnetBlocks[i].GetPos();
						pos2 = magnetBlocks[j].GetPos();

						//setPos[i].x += vecMagToMag.x;
						//setPos[i].y += vecMagToMag.y;
						//setPos[i].z += vecMagToMag.z;
						//setPos[j].x -= vecMagToMag.x;
						//setPos[j].y -= vecMagToMag.y;
						//setPos[j].z -= vecMagToMag.z;

						bMoveVec[i].x += vecMagToMag.x;
						bMoveVec[i].y += vecMagToMag.y;
						bMoveVec[i].z += vecMagToMag.z;

						bMoveVec[j].x -= vecMagToMag.x;
						bMoveVec[j].y -= vecMagToMag.y;
						bMoveVec[j].z -= vecMagToMag.z;

						//magnetBlocks[i].SetPos(pos1);
						//magnetBlocks[j].SetPos(pos2);

					}
				}
				else {
					if (vecLen <= 2.0f) {
						//�x�N�g���𐳋K��+���΂̑����ɒ���
						vecMagToMag = ChangeVec(vecMagToMag, moveSpd);
						//vecMagToMag *= moveSpd;
						//���ꂼ��̃u���b�N�ɉ��Z
						XMFLOAT3 pos1, pos2;
						pos1 = magnetBlocks[i].GetPos();
						pos2 = magnetBlocks[j].GetPos();

						//setPos[i].x -= vecMagToMag.x;
						//setPos[i].y -= vecMagToMag.y;
						//setPos[i].z -= vecMagToMag.z;
						//setPos[j].x += vecMagToMag.x;
						//setPos[j].y += vecMagToMag.y;
						//setPos[j].z += vecMagToMag.z;


						bMoveVec[i].x -= vecMagToMag.x;
						bMoveVec[i].y -= vecMagToMag.y;
						bMoveVec[i].z -= vecMagToMag.z;

						bMoveVec[j].x += vecMagToMag.x;
						bMoveVec[j].y += vecMagToMag.y;
						bMoveVec[j].z += vecMagToMag.z;

						//magnetBlocks[i].SetPos(pos1);
						//magnetBlocks[j].SetPos(pos2);


					}
				}
			}

		}
	}

}

void Colision::MagToPlayerUpdate()
{

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (magnetBlocks[i].GetIsMove()) {

			XMFLOAT3 pos = { setPos[i].x + bMoveVec[i].x,setPos[i].y + bMoveVec[i].y,setPos[i].z + bMoveVec[i].z };

			//���@�̏�Ԃ����΂Ȃ�����񂹓��̏������s��
			if (pState != UnMagnet) {
				bool isPlayerNorth = false;
				if (pState == NorthPole) {
					isPlayerNorth = true;
				}
				else if (pState == SouthPole) {
					isPlayerNorth = false;
				}
				bool isPull;
				if (isPlayerNorth == magnetBlocks[i].GetIsNorth()) {

					isPull = false;
				}
				else {
					isPull = true;
				}

				//���@���W���Q�Ƃ��A���@�Ǝ��΂̋������v�Z
				XMFLOAT3 vecPlayerToblock;
				vecPlayerToblock.x = pPos.x - pos.x;
				vecPlayerToblock.y = pPos.y - pos.y;
				vecPlayerToblock.z = pPos.z - pos.z;

				//�x�N�g���̒����͈ړ��J�n�����ȉ��Ȃ玩�@�A���΂̎��͂��g���Ĉ����񂹓��̏���
				float vecLength = lengthVec(vecPlayerToblock);

				float moveSpd = 0.025;


				ImGui::Begin(" vecPlayerToblock");
				ImGui::Text("[%d] = (%f,%f,%f) \n", i, vecPlayerToblock.x, vecPlayerToblock.y, vecPlayerToblock.z);
				ImGui::End();


				//�����Ŏ��͂̋�����ω�������
				if (2.0f > vecLength) {
					moveSpd = ((2.0f / 1000) - (vecLength / 1000)) / 0.01;
				}

				if (isPull) {

					if (vecLength <= 2.0f) {
						XMFLOAT3 moveVec;

						moveVec = ChangeVec(vecPlayerToblock, moveSpd);
						//moveVec *= moveSpd;
						//setPos[i].x += moveVec.x;
						//setPos[i].y += moveVec.y;
						//setPos[i].z += moveVec.z;

						bMoveVec[i].x += moveVec.x;
						bMoveVec[i].y += moveVec.y;
						bMoveVec[i].z += moveVec.z;


					}
				}
				else {

					if (vecLength <= 2.0f) {
						XMFLOAT3 moveVec;
						moveVec = ChangeVec(vecPlayerToblock, moveSpd);

						//moveVec *= moveSpd;
						//setPos[i].x -= moveVec.x;
						//setPos[i].y -= moveVec.y;
						//setPos[i].z -= moveVec.z;

						bMoveVec[i].x -= moveVec.x;
						bMoveVec[i].y -= moveVec.y;
						bMoveVec[i].z -= moveVec.z;
					}
				}
			}

			//magnetBlocks[i].SetPos(pos);

		}

	}
}

void Colision::MagnetPower()
{
	//���͂�ON,OFF

	//4�ʒ��ׂĂ��������Ă�������̎��΂Ƃ͔������Ȃ��悤��


	float bSize = 2; // (2 * 0.99)

	float pSize = 2;

	int isPower[20] = {};

	//�z��̍ő吔-1��for������
	for (int i = 0; i < magnetBlocks.size() - 1; i++) {

		//i�ڂ̎��΂ɑ΂��āAi+1 ~ �z�񖖔��܂ł̃u���b�N�Ǝ��΂̔�����s��
		for (int j = i + 1; j < magnetBlocks.size(); j++) {

			for (int k = 1; k < 5; k++) {

				////���@�Ǝ���

				if (magnetBlocks[i].GetContactNum(k) == i) {

					/*			debugText_->SetPos(0, 80);
								debugText_->Printf("qqqqqqqqqqqq ");*/

					if (k == 1) {
						if (magnetBlocks[i].GetPos().z + (bSize / 2) + bMoveVec[i].z <= player->GetPosition().z + bMoveVec[j].z) {
							magnetBlocks[i].SetIsMove(false);

						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}

					if (k == 2) {
						if (magnetBlocks[i].GetPos().z - (bSize / 2) + bMoveVec[i].z > player->GetPosition().z + bMoveVec[j].z) {
							magnetBlocks[i].SetIsMove(false);
						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}


					if (k == 3) {
						if (magnetBlocks[i].GetPos().x - (bSize / 2) + bMoveVec[i].x >= player->GetPosition().x + bMoveVec[j].x) {
							magnetBlocks[i].SetIsMove(false);

						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}


					if (k == 4) {
						if (magnetBlocks[i].GetPos().x + (bSize / 2) + bMoveVec[i].x < player->GetPosition().x + bMoveVec[j].x) {
							magnetBlocks[i].SetIsMove(false);
						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}

				}
				else {
					magnetBlocks[i].SetIsMove(true);
				}

				//���Γ��m

				if (magnetBlocks[i].GetContactNum(k) == 100) {
					continue;
				}

				//debugText_->SetPos(0, 40);
				//debugText_->Printf("qqqqqqqqqqqq ");

				if (magnetBlocks[i].GetContactNum(k) != i) {

					if (magnetBlocks[i].GetContactNum(k) != j) {

						if (k == 1) {
							if (setPos[i].z + (bSize / 2) + bMoveVec[i].z < setPos[j].z + bMoveVec[j].z) {
								magnetBlocks[i].SetIsMagMove(j, false);
								isPower[i] = 1;
							}
							else {
								if (isPower[i] == 0) {
									magnetBlocks[i].SetIsMagMove(j, true);
								}
							}
						}

						else if (k == 2) {
							if (setPos[i].z - (bSize / 2) + bMoveVec[i].z > setPos[j].z + bMoveVec[j].z) {
								magnetBlocks[i].SetIsMagMove(j, false);
								isPower[i] = 1;

							}
							else {
								if (isPower[i] == 0) {
									magnetBlocks[i].SetIsMagMove(j, true);
								}
							}
						}


						else if (k == 3) {
							if (setPos[i].x - (bSize / 2) + bMoveVec[i].x >= setPos[j].x + bMoveVec[j].x) {
								magnetBlocks[i].SetIsMagMove(j, false);
								isPower[i] = 1;

							}
							else {
								if (isPower[i] == 0) {
									magnetBlocks[i].SetIsMagMove(j, true);
								}
							}
						}


						else if (k == 4) {
							if (setPos[i].x + (bSize / 2) + bMoveVec[i].x < setPos[j].x + bMoveVec[j].x) {
								magnetBlocks[i].SetIsMagMove(j, false);
								isPower[i] = 1;


							}
							else {
								if (isPower[i] == 0) {
									magnetBlocks[i].SetIsMagMove(j, true);
								}
							}
						}



					}
					//else if (magnetBlocks[i].GetContactNum(k) == j) {


					//	//if (k == 1 || k == 2) {
					//	//	if (magnetBlocks[i].GetPos().x + 0.1 <= magnetBlocks[j].GetPos().x || magnetBlocks[i].GetPos().x - 0.1 >= magnetBlocks[j].GetPos().x) {
					//	//		magnetBlocks[i].SetIsMagMove(j, false);
					//	//		isPower[i] = k;
					//	//		//setPos[i].x = setPos[j].x;
					//	//	}
					//	//	//else {
					//	//	//	if (isPower[i] == 0) {
					//	//	//		magnetBlocks[i].SetIsMagMove(j, true);
					//	//	//	}
					//	//	//}
					//	//}



					//}

				}

			}

		}
	}


}

void Colision::InforUpdate()
{

	//n�u���b�N ���W

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//�u���b�N ���W

		bPos[i] = magnetBlocks[i].GetPos();

		bPosX1[i] = bPos[i].x - (bSize / 2);
		bPosX2[i] = bPos[i].x + (bSize / 2);

		bPosZ1[i] = bPos[i].z - (bSize / 2);
		bPosZ2[i] = bPos[i].z + (bSize / 2);

		setPos[i] = magnetBlocks[i].GetPos();

		for (int j = 0; j < 5; j++) {
			magnetBlocks[i].ReSetContactNum(j);
		}


		bMoveVec[i] = { 0,0,0 };

	}

	//--------���@----------

	pPos = player->GetPosition();

	pSize = player->GetSize();
	pState = player->GetState();

	pPosX1 = pPos.x - (pSize / 2);
	pPosX2 = pPos.x + (pSize / 2);

	pPosZ1 = pPos.z - (pSize / 2);
	pPosZ2 = pPos.z + (pSize / 2);

	pMoveVec = player->GetMove();
}

void Colision::StickMag()
{


	//bool stickBlockMass[20];

	//�������Ă���u���b�N��true��
	for (int i = 0; i < magnetBlocks.size(); i++) {


		//stickBlockMass[i] = false;

		for (int k = 1; k < 5; k++) {

			//�����������Ă���Ȃ�
			if (magnetBlocks[i].GetContactNum(k) != 100 && magnetBlocks[i].GetContactNum(k) != i) {

				if (k == 1 || k == 2) {


					float diff = (setPos[i].x + bMoveVec[i].x) - (setPos[magnetBlocks[i].GetContactNum(k)].x + bMoveVec[magnetBlocks[i].GetContactNum(k)].x);

					//���S�ɂ��������炭��������true��
					if (diff < 0.01f && diff > -0.01f) {

						stickBlockMass[i] = true;

						stickBlockMass[magnetBlocks[i].GetContactNum(k)] = true;

					}

				}

				if (k == 3 || k == 4) {

					float diff = (setPos[i].z + bMoveVec[i].z) - (setPos[magnetBlocks[i].GetContactNum(k)].z + bMoveVec[magnetBlocks[i].GetContactNum(k)].z);

					if (diff < 0.01f && diff > -0.01f) {

						stickBlockMass[i] = true;

						stickBlockMass[magnetBlocks[i].GetContactNum(k)] = true;

					}

				}


				//stickBlockMass[i] = true;

				//stickBlockMass[magnetBlocks[i].GetContactNum(k)] = true;

			}
		}
	}

	XMFLOAT3 massMoveVec = {};

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (stickBlockMass[i]) {

			massMoveVec.x += bMoveVec[i].x;
			massMoveVec.y += bMoveVec[i].y;
			massMoveVec.z += bMoveVec[i].z;


		}

	}

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (stickBlockMass[i]) {

			bMoveVec[i] = massMoveVec;

		}

	}

	for (int i = 0; i < magnetBlocks.size(); i++) {

		for (int j = 0; j < magnetBlocks.size(); j++) {

			//debugText_->SetPos((200 * j), 140 + (20 * i));
			//debugText_->Printf("%d", magnetBlocks[j].GetIsMagMove(i));

		}
	}

	//debugText_->SetPos(0, 200);
	//debugText_->Printf("adsddadad");

	////���������炻�̃u���b�N���痣��Ȃ��悤�ɂ��鏈��

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	//�ق��̎��΂ɂ������Ă���Ƃ�
	//	if (magnetBlocks[i].GetIsStick() == true) {

	//		XMFLOAT3 setPos = magnetBlocks[i].GetPos();
	//		contact = magnetBlocks[i].GetIsStickContact();

	//		int stickBlockNum = magnetBlocks[i].GetIsStickBlockNum();
	//		XMFLOAT3 stickPos = magnetBlocks[stickBlockNum].GetPos();

	//		int blockSize = 2;

	//		//�����������̏���
	//		if (contact == 1) {
	//			setPos.z = (stickPos.z - blockSize);
	//			setPos.x = stickPos.x;
	//		}
	//		else if (contact == 2) {
	//			setPos.z = (stickPos.z + blockSize);
	//			setPos.x = stickPos.x;
	//		}
	//		else if (contact == 3) {
	//			setPos.x = (stickPos.x + blockSize);
	//			setPos.z = stickPos.z;
	//		}
	//		else if (contact == 4) {
	//			setPos.x = (stickPos.x - blockSize);
	//			setPos.z = stickPos.z;
	//		}

	//		////�v�Z�������W���Z�b�g
	//		magnetBlocks[i].SetPos(setPos);

	//	}
	//	else {

	//	}

	//}

}

int Colision::GetContact(XMFLOAT3 mainPos, XMFLOAT3 subPos)
{
	//0�Ȃ� 1��@2���@3���@4�E
	int contact = 0;

	//X�� Z�����f�p
	float contactNumX = 0;
	float contactNumZ = 0;

	if (mainPos.x > subPos.x) {
		contact = 4;
		contactNumX = mainPos.x - subPos.x;
	}
	else {
		contact = 3;
		contactNumX = subPos.x - mainPos.x;
	}

	if (mainPos.z > subPos.z) {
		contactNumZ = mainPos.z - subPos.z/* + 0.05*/;
	}
	else {
		contactNumZ = subPos.z - mainPos.z/* + 0.05*/;
	}

	if (contactNumX < contactNumZ) {

		if (mainPos.z > subPos.z) {
			contact = 1;
		}
		else {
			contact = 2;
		}

	}

	return contact;
}

XMFLOAT3 Colision::GetVec(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	return XMFLOAT3(pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z);
}

XMFLOAT3 Colision::ChangeVec(XMFLOAT3 changeVec, float size)
{
	//���K��

	XMFLOAT3 vec = changeVec;

	float l = 1 / lengthVec(vec);

	vec.x *= l;
	vec.y *= l;
	vec.z *= l;

	//size��������

	vec.x *= size;
	vec.y *= size;
	vec.z *= size;


	return vec;
}

float Colision::lengthVec(XMFLOAT3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}