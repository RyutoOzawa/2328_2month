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


	//ゲームで使うようの配列に格納
	magnetBlocks.push_back(magnetBlock);
	magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);

}

void Colision::Update() {

	//磁力切り替え
	MagnetPower();

	//座標関係更新
	InforUpdate();

	//磁石の移動
	MagnetsUpdate();

	//ブロックとの当たり判定
	MapCollision();

	//磁石と自機の当たり判定(押し戻し処理)
	PosCollision();

	//くっついてるブロックのベクトルを統一
	StickMag();

	//座標の更新
	for (int i = 0; i < magnetBlocks.size(); i++) {
		//計算した座標を確定

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

//判定
void Colision::MapCollision()
{

	//当たらないよう調整する用
	float adjustPixcelSpeed = player->GetAdjustPixelSpeed();
	//自機サイズ調整用
	float adjustPlayerSize = 0.03;

	//座標を用意
	float leftplayer = player->GetPosition().x + adjustPlayerSize;
	float downplayer = player->GetPosition().y + adjustPlayerSize;
	float frontplayer = player->GetPosition().z + adjustPlayerSize;

	float rightplayer = player->GetPosition().x + player->GetSize() - adjustPlayerSize;
	float upplayer = player->GetPosition().y - player->GetSize() - adjustPlayerSize;
	float backplayer = player->GetPosition().z + player->GetSize() - adjustPlayerSize;

	//当たっているか
	//Vector2 ColX = { 0,0 };
	//Vector2 ColY = { 0,0 };
	//Vector2 ColZ = { 0,0 };

	float playerSpeed = player->GetSpeed() + adjustPixcelSpeed;

	/////////////
	//プレイヤー//
	/////////////


	//右に仮想的に移動して当たったら
	if (savemap_->mapcol(rightplayer + playerSpeed, downplayer + player->GetSize() / 2, frontplayer) || savemap_->mapcol(rightplayer + playerSpeed, downplayer + player->GetSize() / 2, backplayer))
	{

		if (player->GetMove().x > 0 && ColX.x == 0) {
			//１ピクセル先に壁が来るまで移動
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

	//左に仮想的に移動して当たったら
	if (savemap_->mapcol(leftplayer - playerSpeed, downplayer + player->GetSize() / 2, frontplayer) || savemap_->mapcol(leftplayer - playerSpeed, downplayer + player->GetSize() / 2, backplayer))
	{
		if (player->GetMove().x < 0 && ColX.y == 0) {
			//１ピクセル先に壁が来るまで移動
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


	//z軸に対しての当たり判定
	//奥に仮想的に移動して当たったら
	if (savemap_->mapcol(leftplayer, downplayer + player->GetSize() / 2, backplayer + playerSpeed) || savemap_->mapcol(rightplayer, downplayer + player->GetSize() / 2, backplayer + playerSpeed))
	{
		if (player->GetMove().z > 0 && ColZ.x == 0) {
			//１ピクセル先に壁が来るまで移動
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

	//手前に仮想的に移動して当たったら
	if (savemap_->mapcol(leftplayer, downplayer + player->GetSize() / 2, frontplayer - playerSpeed) || savemap_->mapcol(rightplayer, downplayer + player->GetSize() / 2, frontplayer - playerSpeed))
	{
		if (player->GetMove().z < 0 && ColZ.y == 0) {
			//１ピクセル先に壁が来るまで移動
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

		//ブロック 座標

		bPos[i] = setPos[i];

		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;



		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;


	}

	//自機と磁石

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

			if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

				XMFLOAT3 pos1 = pPos;
				XMFLOAT3 pos2 = { setPos[i].x + bMoveVec[i].x,setPos[i].y + bMoveVec[i].y ,setPos[i].z + bMoveVec[i].z };

				float pos1Size = pSize;
				float pos2Size = bSize;

				//調整用
				XMFLOAT3 adjust = GetVec(pos1, pos2);

				adjust = ChangeVec(adjust, 0.0001f);

				int contact = GetContact(pos1, pos2);

				if (pState != UnMagnet) {

					//押し戻し処理
					while (true)
					{
						if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

							//まだ重なっていたら離す

							//あたったのがx軸かz軸かで押し戻しを変更
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

					//当たってないところでセット
					//player->SetPos(pos1);
					//setPos[i] = pos2;

					//プレイヤー移動分を加算
					bMoveVec[i].x += pMoveVec.x;
					bMoveVec[i].z += pMoveVec.z;

					//あったった面を記録
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


					////押し戻し処理
					//while (true)
					//{
					//	if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

					//		//まだ重なっていたら離す

					//		//あたったのがx軸かz軸かで押し戻しを変更
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

					//当たってないところでセット
					//player->SetPos(pos1);
					// 
					//自機が磁力なしの場合めり込ませないように
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

	//----------磁石ブロックと磁石ブロック----------

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ブロック 座標


		bPos[i] = setPos[i];


		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;



		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;

	}

	//もう当たったかどうか判断するようの配列

	//あったったときの情報などを取得する

	for (int i = 0; i < magnetBlocks.size(); i++) {

		for (int j = 0; j < magnetBlocks.size(); j++) {

			//同じ磁石ともう処理した組み合わせは当たった処理をしないように

			//同じ磁石か

			if (j >= i) {
				break;
			}

			XMFLOAT3 pos1 = { setPos[i].x + bMoveVec[i].x, setPos[i].y + bMoveVec[i].y ,setPos[i].z + bMoveVec[i].z };

			XMFLOAT3 pos2 = { setPos[j].x + bMoveVec[j].x, setPos[j].y + bMoveVec[j].y ,setPos[j].z + bMoveVec[j].z };

			float pos1Size = bSize;
			float pos2Size = bSize;

			//調整用
			XMFLOAT3 adjust = GetVec(pos1, pos2);

			adjust = ChangeVec(adjust, 0.0001f);

			int contact1 = GetContact(pos2, pos1);
			int contact2 = GetContact(pos1, pos2);


			if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

				if (bPosZ1[j] < bPosZ2[i] && bPosZ1[i] < bPosZ2[j]) {

					//XMFLOAT3 pos1 = magnetBlocks[i].GetPos();
					//XMFLOAT3 pos2 = magnetBlocks[j].GetPos();

					//押し戻し処理
					while (true)
					{
						if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

							//あたったのがx軸かz軸かで押し戻しを変更
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


					//当たってないところでセット
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


	//自機と磁石座標変更で移動制限も変更

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ブロック 座標

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

					////あったった面を記録
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


					//自機が磁力なしの場合めり込ませないように
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

	//配列の最大数-1回for文を回す
	for (int i = 0; i < magnetBlocks.size() - 1; i++) {

		//i個目の磁石に対して、i+1 ~ 配列末尾までのブロックと磁石の判定を行う
		for (int j = i + 1; j < magnetBlocks.size(); j++) {

			// 引き寄せ処理がどちらもONの場合引き寄せるように
			bool isMagMove = magnetBlocks[i].GetIsMagMove(j);

			if (isMagMove) {
				isMagMove = magnetBlocks[j].GetIsMagMove(i);
			}


			if (isMagMove) {

				bool isSame = false;
				//同極か対極か
				if (magnetBlocks[i].GetIsNorth() == magnetBlocks[j].GetIsNorth()) {
					isSame = true;
				}

				//ブロック同士のベクトル作成
				XMFLOAT3 vecMagToMag;
				vecMagToMag.x = (magnetBlocks[i].GetPos().x) - (magnetBlocks[j].GetPos().x);
				vecMagToMag.y = (magnetBlocks[i].GetPos().y) - (magnetBlocks[j].GetPos().y);
				vecMagToMag.z = (magnetBlocks[i].GetPos().z) - (magnetBlocks[j].GetPos().z);

				//ベクトルの長さ取得
				float vecLen = lengthVec(vecMagToMag);

				float moveSpd = 0.025;

				//距離で磁力の強さを変化させる
				if (2.0f > vecLen) {
					moveSpd = ((2.0f / 1000) - (vecLen / 1000)) / 0.01;
				}



				//i個目の磁石とj個目の磁力による挙動
				if (isSame) {
					if (vecLen <= 2.0f) {



						//ベクトルを正規化+磁石の速さに直す
						vecMagToMag = ChangeVec(vecMagToMag, moveSpd);
						//vecMagToMag *= moveSpd;
						//それぞれのブロックに加算
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
						//ベクトルを正規化+磁石の速さに直す
						vecMagToMag = ChangeVec(vecMagToMag, moveSpd);
						//vecMagToMag *= moveSpd;
						//それぞれのブロックに加算
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

			//自機の状態が磁石なら引き寄せ等の処理を行う
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

				//自機座標を参照し、自機と磁石の距離を計算
				XMFLOAT3 vecPlayerToblock;
				vecPlayerToblock.x = pPos.x - pos.x;
				vecPlayerToblock.y = pPos.y - pos.y;
				vecPlayerToblock.z = pPos.z - pos.z;

				//ベクトルの長さは移動開始距離以下なら自機、磁石の磁力を使って引き寄せ等の処理
				float vecLength = lengthVec(vecPlayerToblock);

				float moveSpd = 0.025;


				ImGui::Begin(" vecPlayerToblock");
				ImGui::Text("[%d] = (%f,%f,%f) \n", i, vecPlayerToblock.x, vecPlayerToblock.y, vecPlayerToblock.z);
				ImGui::End();


				//距離で磁力の強さを変化させる
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
	//磁力のON,OFF

	//4面調べてあったっている方向の磁石とは反応しないように


	float bSize = 2; // (2 * 0.99)

	float pSize = 2;

	int isPower[20] = {};

	//配列の最大数-1回for文を回す
	for (int i = 0; i < magnetBlocks.size() - 1; i++) {

		//i個目の磁石に対して、i+1 ~ 配列末尾までのブロックと磁石の判定を行う
		for (int j = i + 1; j < magnetBlocks.size(); j++) {

			for (int k = 1; k < 5; k++) {

				////自機と磁石

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

				//磁石同士

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

	//nブロック 座標

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ブロック 座標

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

	//--------自機----------

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

	//くっついているブロックをtrueに
	for (int i = 0; i < magnetBlocks.size(); i++) {


		//stickBlockMass[i] = false;

		for (int k = 1; k < 5; k++) {

			//もしくっついているなら
			if (magnetBlocks[i].GetContactNum(k) != 100 && magnetBlocks[i].GetContactNum(k) != i) {

				if (k == 1 || k == 2) {


					float diff = (setPos[i].x + bMoveVec[i].x) - (setPos[magnetBlocks[i].GetContactNum(k)].x + bMoveVec[magnetBlocks[i].GetContactNum(k)].x);

					//完全にくっついたらくっついたをtrueに
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

	////くっついたらそのブロックから離れないようにする処理

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	//ほかの磁石にくっついているとき
	//	if (magnetBlocks[i].GetIsStick() == true) {

	//		XMFLOAT3 setPos = magnetBlocks[i].GetPos();
	//		contact = magnetBlocks[i].GetIsStickContact();

	//		int stickBlockNum = magnetBlocks[i].GetIsStickBlockNum();
	//		XMFLOAT3 stickPos = magnetBlocks[stickBlockNum].GetPos();

	//		int blockSize = 2;

	//		//当たった時の処理
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

	//		////計算した座標をセット
	//		magnetBlocks[i].SetPos(setPos);

	//	}
	//	else {

	//	}

	//}

}

int Colision::GetContact(XMFLOAT3 mainPos, XMFLOAT3 subPos)
{
	//0なし 1上　2下　3左　4右
	int contact = 0;

	//X軸 Z軸判断用
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
	//正規化

	XMFLOAT3 vec = changeVec;

	float l = 1 / lengthVec(vec);

	vec.x *= l;
	vec.y *= l;
	vec.z *= l;

	//sizeをかける

	vec.x *= size;
	vec.y *= size;
	vec.z *= size;


	return vec;
}

float Colision::lengthVec(XMFLOAT3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}