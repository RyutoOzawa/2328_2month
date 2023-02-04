#include "Colision.h"
#include <cassert>
#include"ImguiManager.h"

Colision::Colision() {}

Colision::~Colision() {
}

void Colision::Initialize(Player* player, MagnetBlock magnetBlock, Map* map, int i) {

	//this->player = player;

	//magnetBlocks[i] = magnetBlock;

	//player = new Player();


	//ゲームで使うようの配列に格納
	magnetBlocks.push_back(magnetBlock);
	magnetBlocks[i].obj.scale = XMFLOAT3(0.1f, 0.1f, 0.1f);
	map_ = map;

}

void Colision::Update() {

	//磁力切り替え
	MagnetPower();

	//座標関係更新
	InforUpdate();

	//磁石の移動
	MagnetsUpdate();

	//磁石と自機の当たり判定(押し戻し処理)
	PosCollision();

	//くっついているブロックを確定
	//SetStickMag();
	//くっついてるブロックのベクトルを統一
	//StickMag();

	MagFall();

	//ブロックとの当たり判定
	MapCollision();

	//MagFall();

	////くっついてるブロックのベクトルを統一
	//StickMag();

	//座標の更新
	for (int i = 0; i < magnetBlocks.size(); i++) {
		//計算した座標を確定

		setPos[i].x += bMoveVec[i].x;
		setPos[i].y += bMoveVec[i].y;
		setPos[i].z += bMoveVec[i].z;

		////ImGui::Begin("setPos");
		////ImGui::Text("%d = %f,%f,%f \n", i, setPos[i].x, setPos[i].y, setPos[i].z);
		////ImGui::End();

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
	float adjustPixcelSpeed = 0.01;

	//座標を用意
	float leftplayer = player->GetPosition().x;
	float downplayer = player->GetPosition().y;
	float frontplayer = player->GetPosition().z;

	float rightplayer = player->GetPosition().x + player->GetSize();
	float upplayer = player->GetPosition().y + player->GetSize();
	float backplayer = player->GetPosition().z + player->GetSize();

	//当たっているか
	//Vector2 ColX = { 0,0 };
	//Vector2 ColY = { 0,0 };
	//Vector2 ColZ = { 0,0 };

	float playerSpeed = player->GetSpeed() + adjustPixcelSpeed;

	/////////////
	//プレイヤー//
	///////////////

	//ImGui::Begin("map");
	//ImGui::Text("player = %f,%f,%f,%f \n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	////ImGui::Text("pPos = %f \n", pSize);
	//ImGui::End();


	//自機落下関係

	//下に仮想的に移動して当たったら


	if (map_->mapcol(leftplayer, downplayer - playerSpeed, frontplayer) || map_->mapcol(rightplayer, downplayer - playerSpeed, backplayer) || map_->mapcol(rightplayer, downplayer - playerSpeed, frontplayer) || map_->mapcol(leftplayer, downplayer - playerSpeed, backplayer))
	{

		player->SetFall(false);
		ImGui::Begin("fall");
		ImGui::Text("false");
		ImGui::End();

	}
	else {

		player->SetFall(true);
		ImGui::Begin("fall");
		ImGui::Text("true");
		ImGui::End();

	}


	//上に仮想的に移動して当たったら
	if (map_->mapcol(leftplayer, upplayer + playerSpeed, frontplayer) || map_->mapcol(rightplayer, upplayer + playerSpeed, backplayer) || map_->mapcol(rightplayer, upplayer + playerSpeed, frontplayer) || map_->mapcol(leftplayer, upplayer + playerSpeed, backplayer))
	{
		//player->SetFall(true);
		player->SetJump(false);

	}
	else {

		//player->SetFall(false);

	}

	//プレイヤーの下にブロックがあったら進めるように

	InforUpdateMagnetPos();

	//当たり判定調整用

	for (int i = 0; i < magnetBlocks.size(); i++) {

		float adjust = 0.2;

		bool leftUp = false;
		bool rightUp = false;
		bool leftDown = false;
		bool rightDown = false;

		//プレイヤーの下辺よりブロックの上辺が下だった場合,ブロックの上にいるとする
		if (pPosY1 > bPosY2[i] - adjust) {

			//左上の点がブロック状にあるか
			if (pPosX1 > bPosX1[i] - adjust && pPosX1 < bPosX2[i] + adjust) {

				if (pPosY1 < bPosY2[i] + adjust) {

					if (pPosZ2 > bPosZ1[i] - adjust && pPosZ2 < bPosZ2[i] + adjust) {

						leftUp = true;

					}
				}
			}

			//右上の点がブロック状にあるか
			if (pPosX2 > bPosX1[i] - adjust && pPosX2 < bPosX2[i] + adjust) {

				if (pPosY1 < bPosY2[i] + adjust) {

					if (pPosZ2 > bPosZ1[i] - adjust && pPosZ2 < bPosZ2[i] + adjust) {

						rightUp = true;

					}

				}
			}

			//左下の点がブロック状にあるか
			if (pPosX1 > bPosX1[i] - adjust && pPosX1 < bPosX2[i] + adjust) {

				if (pPosY1 < bPosY2[i] + adjust) {

					if (pPosZ1 > bPosZ1[i] - adjust && pPosZ1 < bPosZ2[i] + adjust) {

						leftDown = true;

					}
				}

			}

			//右下の点がブロック状にあるか
			if (pPosX2 > bPosX1[i] - adjust && pPosX2 < bPosX2[i] + adjust) {

				if (pPosY1 < bPosY2[i] + adjust) {

					if (pPosZ1 > bPosZ1[i] - adjust && pPosZ1 < bPosZ2[i] + adjust) {

						rightDown = true;

					}
				}

			}



			//左上と右上の点がブロック上にあったら落ちない

			if (leftUp && rightUp) {
				player->SetFall(false);
				//ImGui::Begin("up");
				//ImGui::Text("true");
				//ImGui::End();
			}

			//左下と右下の点がブロック上にあったら落ちない
			if (leftDown && rightDown) {
				player->SetFall(false);

				//ImGui::Begin("rdown");
				//ImGui::Text("true");
				//ImGui::End();
			}

			//左上と左下の点がブロック上にあったら落ちない
			if (leftUp && leftDown) {
				player->SetFall(false);

				//ImGui::Begin("left");
				//ImGui::Text("true");
				//ImGui::End();
			}

			//右上と右下の点がブロック上にあったら落ちない

			if (rightUp && rightDown) {
				player->SetFall(false);

				//ImGui::Begin("right");
				//ImGui::Text("true");
				//ImGui::End();
			}

			//ジャンプするか
			bool isJump = false;

			//当たったら

			int adjust2 = 0.3;

			if (pPosX1 + adjust2 < bPosX2[i] - adjust2 && bPosX1[i] + adjust2 < pPosX2 - adjust2) {

				if (pPosZ1 + adjust2 < bPosZ2[i] - adjust2 && bPosZ1[i] + adjust2 < pPosZ2 - adjust2) {

					isJump = true;

				}
			}

			if (isJump) {

				//もし自機と同じ面だったらジャンプさせる
				if (pState == NorthPole && magnetBlocks[i].GetIsNorth() == 1) {
					isJump = true;
				}
				else if (pState == SouthPole && magnetBlocks[i].GetIsNorth() == 0) {
					isJump = true;
				}
				else {
					isJump = false;
				}

				if (isJump && pPosY1 < bPosY2[i] + (bSize / 2)) {

					player->SetJump(true);

				}

				////プレイヤーが高さ上限まで飛んだら
				//if (pPosY1 >= player->GetJumpHigjt()) {
				//	player->SetJump(false);
				//}

			}

		}

		//ImGui::Begin("bY");
		//ImGui::Text("bPos[%d] = %f,%f \n", i, bPosY1[i], bPosY2[i]);
		//ImGui::End();

	}


	//ImGui::Begin("PY");
	//ImGui::Text("pPos = %f,%f \n", pPosY1, pPosY2);
	//ImGui::End();

	//ImGui::Begin("COL");
	//ImGui::Text("X.x = %f,X.y = %f,Z.x = %f,Z.y = %f \n", ColX.x, ColX.y, ColZ.x, ColZ.y);
	//ImGui::End();


	//player->SetColX(ColX);
	//player->SetColY(ColY);
	//player->SetColZ(ColZ);


	//右に仮想的に移動して当たったら
	if (map_->mapcol(rightplayer + playerSpeed, downplayer, frontplayer) || map_->mapcol(rightplayer + playerSpeed, downplayer, backplayer) || map_->mapcol(rightplayer + playerSpeed, upplayer, frontplayer) || map_->mapcol(rightplayer + playerSpeed, upplayer, backplayer))
	{

		if (player->GetMove().x > 0 && ColX.x == 0) {
			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapcol(rightplayer + adjustPixcelSpeed, downplayer, frontplayer) || map_->mapcol(rightplayer + adjustPixcelSpeed, downplayer, backplayer) || map_->mapcol(rightplayer + adjustPixcelSpeed, upplayer, frontplayer) || map_->mapcol(rightplayer + adjustPixcelSpeed, upplayer, backplayer)) {
					break;
				}

				player->OnMapCollisionX2();
				rightplayer = player->GetPosition().x + player->GetSize();
				leftplayer = player->GetPosition().x;

			}

			//ColX.x = 1;
		}
		ColX.x = 1;

	}


	//左に仮想的に移動して当たったら
	if (map_->mapcol(leftplayer - playerSpeed, downplayer, frontplayer) || map_->mapcol(leftplayer - playerSpeed, downplayer, backplayer) || map_->mapcol(leftplayer - playerSpeed, upplayer, frontplayer) || map_->mapcol(leftplayer - playerSpeed, upplayer, backplayer))
	{
		if (player->GetMove().x < 0 && ColX.y == 0) {
			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapcol(leftplayer - adjustPixcelSpeed, downplayer, frontplayer) || map_->mapcol(leftplayer - adjustPixcelSpeed, downplayer, backplayer) || (map_->mapcol(leftplayer - adjustPixcelSpeed, upplayer, frontplayer) || map_->mapcol(leftplayer - adjustPixcelSpeed, upplayer, backplayer))) {
					break;
				}

				player->OnMapCollisionX();
				rightplayer = player->GetPosition().x + player->GetSize();
				leftplayer = player->GetPosition().x;
			}

			//ColX.y = 1;

		}
		ColX.y = 1;

	}

	//z軸に対しての当たり判定
	//奥に仮想的に移動して当たったら
	if (map_->mapcol(leftplayer, downplayer, backplayer + playerSpeed) || map_->mapcol(rightplayer, downplayer, backplayer + playerSpeed) || map_->mapcol(leftplayer, upplayer, backplayer + playerSpeed) || map_->mapcol(rightplayer, upplayer, backplayer + playerSpeed))
	{
		if (player->GetMove().z > 0 && ColZ.x == 0) {
			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapcol(leftplayer, downplayer, backplayer + adjustPixcelSpeed) || map_->mapcol(rightplayer, downplayer, backplayer + adjustPixcelSpeed) || map_->mapcol(leftplayer, upplayer, backplayer + adjustPixcelSpeed) || map_->mapcol(rightplayer, upplayer, backplayer + adjustPixcelSpeed)) {
					break;
				}

				player->OnMapCollisionZ2();
				frontplayer = player->GetPosition().z;
				backplayer = player->GetPosition().z + player->GetSize();
			}
			//ColZ.x = 1;

		}
		ColZ.x = 1;
	}

	//手前に仮想的に移動して当たったら
	if (map_->mapcol(leftplayer, downplayer, frontplayer - playerSpeed) || map_->mapcol(rightplayer, downplayer, frontplayer - playerSpeed) || map_->mapcol(leftplayer, upplayer, frontplayer - playerSpeed) || map_->mapcol(rightplayer, upplayer, frontplayer - playerSpeed))
	{


		ImGui::Begin("col");
		ImGui::Text("true");
		ImGui::End();

		if (player->GetMove().z < 0 && ColZ.y == 0) {
			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapcol(leftplayer, downplayer, frontplayer - adjustPixcelSpeed) || map_->mapcol(rightplayer, downplayer, frontplayer - adjustPixcelSpeed) || map_->mapcol(leftplayer, upplayer, frontplayer - adjustPixcelSpeed) || map_->mapcol(rightplayer, upplayer, frontplayer - adjustPixcelSpeed)) {
					break;
				}

				player->OnMapCollisionZ();
				frontplayer = player->GetPosition().z;
				backplayer = player->GetPosition().z + player->GetSize();
			}

			//ColZ.y = 1;

		}
		ColZ.y = 1;

	}


	player->SetColX(ColX);
	player->SetColY(ColY);
	player->SetColZ(ColZ);



	/////////////
	//ブロック//
	/////////////

	for (int i = 0; i < magnetBlocks.size(); i++) {


		//座標を用意
		float leftmagnetBlocks = magnetBlocks[i].GetPos().x;
		float downmagnetBlocks = magnetBlocks[i].GetPos().y;
		float frontmagnetBlocks = magnetBlocks[i].GetPos().z;

		float rightmagnetBlocks = magnetBlocks[i].GetPos().x + magnetBlocks[i].GetSize();
		float upmagnetBlocks = magnetBlocks[i].GetPos().y + magnetBlocks[i].GetSize();
		float backmagnetBlocks = magnetBlocks[i].GetPos().z + magnetBlocks[i].GetSize();

		float magnetBlocksSpeed = 0.05f;

		//当たらないよう調整する用
		float adjustPixcelSpeed = 0.01;
		//サイズ調整用
		float adjustmagnetBlocksSize = 0.00;

		for (int j = 1; j < 7; j++) {
			magnetBlocks[i].SetRockMove(false, j);
		}

		//右に仮想的に移動して当たったら
		if (map_->mapInGoalCol(rightmagnetBlocks + bMoveVec[i].x, downmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks + bMoveVec[i].x, downmagnetBlocks, backmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks + bMoveVec[i].x, upmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks + bMoveVec[i].x, upmagnetBlocks, backmagnetBlocks))
		{
			ImGui::Begin("x+");
			ImGui::Text("true");
			ImGui::End();
			bMoveVec[i].x = 0;
			magnetBlocks[i].SetRockMove(true, 4);

			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapInGoalCol(rightmagnetBlocks + adjustPixcelSpeed, downmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks + adjustPixcelSpeed, downmagnetBlocks, backmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks + adjustPixcelSpeed, upmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks + adjustPixcelSpeed, upmagnetBlocks, backmagnetBlocks)) {
					break;
				}

				rightmagnetBlocks += adjustPixcelSpeed - adjustmagnetBlocksSize;
				leftmagnetBlocks += adjustPixcelSpeed + adjustmagnetBlocksSize;

				bMoveVec[i].x += adjustPixcelSpeed;

			}

		}

		//左に仮想的に移動して当たったら
		else if (map_->mapInGoalCol(leftmagnetBlocks + bMoveVec[i].x, downmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks + bMoveVec[i].x, downmagnetBlocks, backmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks + bMoveVec[i].x, upmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks + bMoveVec[i].x, upmagnetBlocks, backmagnetBlocks))
		{
			ImGui::Begin("x-");
			ImGui::Text("true");
			ImGui::End();
			bMoveVec[i].x = 0;
			magnetBlocks[i].SetRockMove(true, 3);

			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapInGoalCol(leftmagnetBlocks - adjustPixcelSpeed, downmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks - adjustPixcelSpeed, downmagnetBlocks, backmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks - adjustPixcelSpeed, upmagnetBlocks, frontmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks - adjustPixcelSpeed, upmagnetBlocks, backmagnetBlocks)) {
					break;
				}


				rightmagnetBlocks -= adjustPixcelSpeed - adjustmagnetBlocksSize;
				leftmagnetBlocks -= adjustPixcelSpeed + adjustmagnetBlocksSize;

				bMoveVec[i].x -= adjustPixcelSpeed;
			}


		}

		//落下スピード
		float magFallSpeed = 0.05;

		//下に仮想的に移動して当たったら
		if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks - magFallSpeed, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks - magFallSpeed, backmagnetBlocks))
		{

			bMoveVec[i].y = 0;
			magnetBlocks[i].SetRockMove(true, 2);

			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks - adjustPixcelSpeed, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks - adjustPixcelSpeed, backmagnetBlocks)) {
					break;
				}

				downmagnetBlocks -= adjustPixcelSpeed - adjustmagnetBlocksSize;
				upmagnetBlocks -= adjustPixcelSpeed + adjustmagnetBlocksSize;

				bMoveVec[i].y -= adjustPixcelSpeed;

			}

		}


		//z軸に対しての当たり判定
		//奥に仮想的に移動して当たったら
		if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks, backmagnetBlocks + bMoveVec[i].z) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks, backmagnetBlocks + bMoveVec[i].z) || map_->mapInGoalCol(leftmagnetBlocks, upmagnetBlocks, backmagnetBlocks + bMoveVec[i].z) || map_->mapInGoalCol(rightmagnetBlocks, upmagnetBlocks, backmagnetBlocks + bMoveVec[i].z))
		{

			ImGui::Begin("z+");
			ImGui::Text("true");
			ImGui::End();
			bMoveVec[i].z = 0;

			magnetBlocks[i].SetRockMove(true, 5);

			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks, backmagnetBlocks + adjustPixcelSpeed) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks, backmagnetBlocks + adjustPixcelSpeed) || map_->mapInGoalCol(leftmagnetBlocks, upmagnetBlocks, backmagnetBlocks + adjustPixcelSpeed) || map_->mapInGoalCol(rightmagnetBlocks, upmagnetBlocks, backmagnetBlocks + adjustPixcelSpeed)) {
					break;
				}

				frontmagnetBlocks += adjustPixcelSpeed;
				backmagnetBlocks += adjustPixcelSpeed;

				bMoveVec[i].z += adjustPixcelSpeed;
			}

		}


		//手前に仮想的に移動して当たったら
		else if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks, frontmagnetBlocks + bMoveVec[i].z) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks, frontmagnetBlocks + bMoveVec[i].z) || map_->mapInGoalCol(leftmagnetBlocks, upmagnetBlocks, frontmagnetBlocks + bMoveVec[i].z) || map_->mapInGoalCol(rightmagnetBlocks, upmagnetBlocks, frontmagnetBlocks + bMoveVec[i].z))
		{
			ImGui::Begin("z-");
			ImGui::Text("true");
			ImGui::End();
			bMoveVec[i].z = 0;
			magnetBlocks[i].SetRockMove(true, 6);

			//１ピクセル先に壁が来るまで移動
			while (true)
			{
				if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks, frontmagnetBlocks - adjustPixcelSpeed) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks, frontmagnetBlocks - adjustPixcelSpeed) || map_->mapInGoalCol(leftmagnetBlocks, upmagnetBlocks, frontmagnetBlocks - adjustPixcelSpeed) || map_->mapInGoalCol(rightmagnetBlocks, upmagnetBlocks, frontmagnetBlocks - adjustPixcelSpeed)) {
					break;
				}

				frontmagnetBlocks -= adjustPixcelSpeed;
				backmagnetBlocks -= adjustPixcelSpeed;

				bMoveVec[i].z -= adjustPixcelSpeed;
			}
		}

	}


	//プレイヤーの下にブロックがあったら進めるように



	//プレイヤーと磁石がっくっついている場合

	InforUpdateMagnetPos();

	//あったった面を記録(Setcontact) + そのたあったった時の処理

	//自機と磁石

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	int contact = GetContact(pPos, bPos[i]);

	//	if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

	//		if (pPosY1 < bPosY2[i] && bPosY1[i] < pPosY2) {

	//			if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {


	//				if (pState != UnMagnet) {

	//					if (contact == 1) {
	//						if (magnetBlocks[i].GetRockMove(2)) {
	//							ColZ.y = 1;
	//						}
	//					}
	//					else if (contact == 2) {
	//						if (magnetBlocks[i].GetRockMove(1)) {
	//							ColZ.x = 1;
	//						}
	//					}
	//					else if (contact == 3) {
	//						if (magnetBlocks[i].GetRockMove(4)) {
	//							ColX.x = 1;
	//						}
	//					}
	//					else if (contact == 4) {
	//						if (magnetBlocks[i].GetRockMove(3)) {
	//							ColX.y = 1;
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//	ImGui::Begin("BlockGetRockBefore");
	//	ImGui::Text("up = %d,down = %d,left = %d,righ = %d\n", magnetBlocks[i].GetRockMove(1), magnetBlocks[i].GetRockMove(2), magnetBlocks[i].GetRockMove(3), magnetBlocks[i].GetRockMove(4));
	//	ImGui::End();

	//}

	//くっついているブロックの場合の処理

	//int massRockMove[7] = {};

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	if (stickBlockMass[i]) {

	//		for (int j = 1; j < 7; j++) {

	//			if (magnetBlocks[i].GetRockMove(j)) {
	//				massRockMove[j] = true;
	//			}

	//		}

	//	}

	//}

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	for (int j = 1; j < 7; j++) {

	//		if (massRockMove[j]) {
	//			magnetBlocks[i].SetRockMove(true, j);
	//		}

	//	}

	//}


	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	//x,y,z軸に仮想的に移動して当たったら
	//	if (magnetBlocks[i].GetRockMove(1) || magnetBlocks[i].GetRockMove(2)) {
	//		bMoveVec[i].y = 0;
	//	}
	//	else if (magnetBlocks[i].GetRockMove(3) && bMoveVec[i].x < 0) {
	//		bMoveVec[i].x = 0;
	//	}
	//	else if (magnetBlocks[i].GetRockMove(4) && bMoveVec[i].x > 0) {
	//		bMoveVec[i].x = 0;
	//	}
	//	else if (magnetBlocks[i].GetRockMove(5) && bMoveVec[i].z > 0) {
	//		bMoveVec[i].z = 0;
	//	}
	//	else if (magnetBlocks[i].GetRockMove(6) && bMoveVec[i].z < 0) {
	//		bMoveVec[i].z = 0;
	//	}



	//}

	//磁石と磁石がっくっついている場合

	InforUpdateMagnetPos();

	//磁石と磁石

	for (int i = 0; i < magnetBlocks.size(); i++) {


		//ImGui::Begin("GetRockMove");
		//ImGui::Text("%d = %d \n", i,magnetBlocks[i].GetRockMove(5));
		//ImGui::End();

		for (int j = 0; j < magnetBlocks.size(); j++) {


			if (j >= i) {
				break;
			}

			int contact = GetContact(bPos[j], bPos[i]);

			if (bPosX1[j] - 0.025 < bPosX2[i] + 0.025 && bPosX1[i] - 0.025 < bPosX2[j] + 0.025) {

				if (bPosY1[j] - 0.025 < bPosY2[i] + 0.025 && bPosY1[i] - 0.025 < bPosY2[j] + 0.025) {

					if (bPosZ1[j] - 0.025 < bPosZ2[i] + 0.025 && bPosZ1[i] - 0.025 < bPosZ2[j] + 0.025) {



						if (contact == 1) {
							//if (magnetBlocks[i].GetRockMove(2)) {
							//	ColZ.y = 1;
							//}
						}
						else if (contact == 2) {
							//if (magnetBlocks[i].GetRockMove(1)) {
							//	ColZ.x = 1;
							//}
						}

						if (magnetBlocks[i].GetRockMove(3)) {
							magnetBlocks[j].SetRockMove(true, 3);
						}

						if (magnetBlocks[i].GetRockMove(4)) {
							magnetBlocks[j].SetRockMove(true, 4);
						}

						if (magnetBlocks[i].GetRockMove(5)) {
							magnetBlocks[j].SetRockMove(true, 5);
						}

						if (magnetBlocks[i].GetRockMove(6)) {
							magnetBlocks[j].SetRockMove(true, 6);
						}


						if (magnetBlocks[j].GetRockMove(3)) {
							magnetBlocks[i].SetRockMove(true, 3);
						}

						if (magnetBlocks[j].GetRockMove(4)) {
							magnetBlocks[i].SetRockMove(true, 4);
						}

						if (magnetBlocks[j].GetRockMove(5)) {
							magnetBlocks[i].SetRockMove(true, 5);
						}

						if (magnetBlocks[j].GetRockMove(6)) {
							magnetBlocks[i].SetRockMove(true, 6);
						}

					}
				}
			}
		}
	}

	for (int i = 0; i < magnetBlocks.size(); i++) {

		for (int j = 0; j < magnetBlocks.size(); j++) {


			//if (j >= i) {
			//	break;
			//}

			int contact = GetContact(bPos[j], bPos[i]);

			if (bPosX1[j] - 0.025 < bPosX2[i] + 0.025 && bPosX1[i] - 0.025 < bPosX2[j] + 0.025) {

				if (bPosY1[j] - 0.025 < bPosY2[i] + 0.025 && bPosY1[i] - 0.025 < bPosY2[j] + 0.025) {

					if (bPosZ1[j] - 0.025 < bPosZ2[i] + 0.025 && bPosZ1[i] - 0.025 < bPosZ2[j] + 0.025) {



						if (contact == 1) {
							//if (magnetBlocks[i].GetRockMove(2)) {
							//	ColZ.y = 1;
							//}
						}
						else if (contact == 2) {
							//if (magnetBlocks[i].GetRockMove(1)) {
							//	ColZ.x = 1;
							//}
						}

						if (magnetBlocks[i].GetRockMove(3)) {
							magnetBlocks[j].SetRockMove(true, 3);
						}

						if (magnetBlocks[i].GetRockMove(4)) {
							magnetBlocks[j].SetRockMove(true, 4);
						}

						if (magnetBlocks[i].GetRockMove(5)) {
							magnetBlocks[j].SetRockMove(true, 5);
						}

						if (magnetBlocks[i].GetRockMove(6)) {
							magnetBlocks[j].SetRockMove(true, 6);
						}


						if (magnetBlocks[j].GetRockMove(3)) {
							magnetBlocks[i].SetRockMove(true, 3);
						}

						if (magnetBlocks[j].GetRockMove(4)) {
							magnetBlocks[i].SetRockMove(true, 4);
						}

						if (magnetBlocks[j].GetRockMove(5)) {
							magnetBlocks[i].SetRockMove(true, 5);
						}

						if (magnetBlocks[j].GetRockMove(6)) {
							magnetBlocks[i].SetRockMove(true, 6);
						}

					}
				}
			}
		}



	}


	//プレイヤーと磁石がっくっついている場合

	InforUpdateMagnetPos();

	//あったった面を記録(Setcontact) + そのたあったった時の処理

	//自機と磁石

	for (int i = 0; i < magnetBlocks.size(); i++) {

		int contact = GetContact(pPos, bPos[i]);

		if (pPosX1 - 0.1 < bPosX2[i] && bPosX1[i] - 0.1 < pPosX2) {

			if (pPosY1 < bPosY2[i] && bPosY1[i] < pPosY2) {

				if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {


					if (pState != UnMagnet) {

						if (contact == 1) {
							//if (magnetBlocks[i].GetRockMove(2)) {
							//	ColZ.y = 1;
							//}
						}
						else if (contact == 2) {
							//if (magnetBlocks[i].GetRockMove(1)) {
							//	ColZ.x = 1;
							//}
						}
						else if (contact == 3) {
							if (magnetBlocks[i].GetRockMove(4)) {
								ColX.x = 1;
							}
						}
						else if (contact == 4) {
							if (magnetBlocks[i].GetRockMove(3)) {
 								ColX.y = 1;
							}
						}
						else if (contact == 5) {
							if (magnetBlocks[i].GetRockMove(6)) {
								ColZ.y = 1;
							}
						}
						else if (contact == 6) {
							if (magnetBlocks[i].GetRockMove(5)) {
								ColZ.x = 1;

							}
						}
					}

					ImGui::Begin("GetRockMove");
					ImGui::Text("%d = %d \n", i, magnetBlocks[i].GetRockMove(3));
					ImGui::End();


				}
			}

		}
	}


	////磁石と磁石がっくっついている場合

	//InforUpdateMagnetPos();

	////あったった面を記録(Setcontact) + そのたあったった時の処理

	////磁石と磁石

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	for (int j = 0; j < magnetBlocks.size(); j++) {

	//		int contact = GetContact(bPos[j], bPos[i]);

	//		if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

	//			if (bPosY1[j] < bPosY2[i] && bPosY1[i] <bPosY2[j]) {

	//				if (bPosZ1[j] < bPosZ2[i] && bPosZ1[i] < bPosZ2[j]) {


	//					if (pState != UnMagnet) {

	//						if (contact == 1) {
	//							//if (magnetBlocks[i].GetRockMove(2)) {
	//							//	ColZ.y = 1;
	//							//}
	//						}
	//						else if (contact == 2) {
	//							//if (magnetBlocks[i].GetRockMove(1)) {
	//							//	ColZ.x = 1;
	//							//}
	//						}
	//						else if (contact == 3) {
	//							if (magnetBlocks[i].GetRockMove(4)) {
	//								ColX.x = 1;
	//							}
	//						}
	//						else if (contact == 4) {
	//							if (magnetBlocks[i].GetRockMove(3)) {
	//								ColX.y = 1;
	//							}
	//						}
	//						else if (contact == 5) {
	//							if (magnetBlocks[i].GetRockMove(6)) {
	//								ColZ.y = 1;
	//							}
	//						}
	//						else if (contact == 6) {
	//							if (magnetBlocks[i].GetRockMove(5)) {
	//								ColZ.x = 1;

	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}


	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	ImGui::Begin("BlockGetRock");
	//	ImGui::Text("up = %d,down = %d,left = %d,righ = %d Zup = %d,Zdown = %d\n", magnetBlocks[i].GetRockMove(1), magnetBlocks[i].GetRockMove(2), magnetBlocks[i].GetRockMove(3), magnetBlocks[i].GetRockMove(4), magnetBlocks[i].GetRockMove(5), magnetBlocks[i].GetRockMove(6));
	//	ImGui::End();

	//}

	//ImGui::Begin("PlayerGetRock");
	//ImGui::Text("up = %f,down = %f,left = %f,righ = %f\n", ColZ.x, ColZ.y, ColX.y, ColX.x);
	//ImGui::End();

	player->SetColX(ColX);
	player->SetColY(ColY);
	player->SetColZ(ColZ);
}

void Colision::PosCollision()
{

	//ImGui::Begin("player");
	//ImGui::Text("player = %f,%f,%f \n", player->GetPosition().x, player->GetPosition().y, player->GetPosition().z);
	////ImGui::Text("pPos = %f \n", pSize);
	//ImGui::End();

	InforUpdateMagnetPos();

	//自機と磁石

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

			if (pPosY1 < bPosY2[i] - 0.025 && bPosY1[i] < pPosY2) {

				if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

					XMFLOAT3 pos1 = pPos;
					//pos1.x = pPos.x + 0.1;
					//pos1.z = pPos.z + 0.1;

					XMFLOAT3 pos2 = { setPos[i].x + bMoveVec[i].x,setPos[i].y + bMoveVec[i].y ,setPos[i].z + bMoveVec[i].z };

					float pos1Size = pSize;
					float pos2Size = bSize;

					//調整用
					XMFLOAT3 adjust = GetVec(pos1, pos2);

					adjust = ChangeVec(adjust, 0.0001f);

					int contact = GetContact(pos1, pos2);


					//ImGui::Begin("aa");
					//ImGui::Text("aaaa");
					//ImGui::End();

					if (pState != UnMagnet) {

						//ImGui::Begin("aa");
						//ImGui::Text("aaaa");
						//ImGui::End();

						//押し戻し処理
						while (true)
						{
							if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

								//まだ重なっていたら離す

								//あたったのがx軸かz軸かで押し戻しを変更
								if (contact == 1 || contact == 2) {

									break;
									//pos2.x += adjust.x;
									//bMoveVec[i].x += adjust.x;


								}
								else if (contact == 3 || contact == 4) {

									pos2.x += adjust.x;
									bMoveVec[i].x += adjust.x;

								}
								else if (contact == 5 || contact == 6) {

									pos2.z += adjust.z;
									bMoveVec[i].z += adjust.z;

								}
							}
							else {
								magnetBlocks[i].SetRockMove(true, contact);
								break;
							}

						}

						//プレイヤー移動分を加算

						bMoveVec[i].x += pMoveVec.x;
						bMoveVec[i].z += pMoveVec.z;

						if (contact == 1) {
							//ColY.x = 1;


						}
						else if (contact == 2) {
							//ColY.y = 1;

						}

					}
					else {


						//自機が磁力なしの場合めり込ませないように
						if (contact == 1) {
							ColY.x = 1;
						}
						else if (contact == 2) {
							ColY.y = 1;
						}
						else if (contact == 3) {
							ColX.x = 1;
						}
						else if (contact == 4) {
							ColX.y = 1;
						}
						else if (contact == 5) {
							ColZ.y = 1;


							/*	ImGui::Begin("up");
								ImGui::Text("true");
								ImGui::End();*/
						}
						else if (contact == 6) {
							ColZ.x = 1;
						}

						magnetBlocks[i].ReSetContactNum(contact);

					}

				}

			}

		}
	}

	InforUpdateMagnetPos();

	//あったった面を記録(Setcontact) + そのたあったった時の処理

	//自機と磁石

	for (int i = 0; i < magnetBlocks.size(); i++) {

		int contact = GetContact(pPos, bPos[i]);

		if (pPosX1 - 0.01 < bPosX2[i] + 0.01 && bPosX1[i] - 0.01 < pPosX2 + 0.01) {

			if (pPosY1 - 0.01 < bPosY2[i] + 0.01 && bPosY1[i] - 0.01 < pPosY2 + 0.01) {


				if (pPosZ1 - 0.01 < bPosZ2[i] + 0.01 && bPosZ1[i] - 0.01 < pPosZ2 + 0.01) {


					//if (contact == 1) {

					//	//if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

					//	//	if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

					//	//		//上に0.1移動してあったっていたら

					//	//		if (pPosY1 < bPosY2[i] + 0.01 && bPosY1[i] + 0.01 < pPosY2) {


					//	//			//あったった面を記録
					//	//			if (pState == NorthPole && magnetBlocks[i].GetIsNorth() == 0) {
					//	//				magnetBlocks[i].SetContactNum(contact, i);
					//	//			}
					//	//			else if (pState == SouthPole && magnetBlocks[i].GetIsNorth() == 1) {
					//	//				magnetBlocks[i].SetContactNum(contact, i);
					//	//			}
					//	//			else {
					//	//				magnetBlocks[i].ReSetContactNum(contact);
					//	//			}

					//	//		}
					//	//	}

					//	//}

					//}
					//else if (contact == 2) {


					//	//if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

					//	//	if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

					//	//		//上に0.1移動してあったっていたら

					//	//		if (pPosY1 < bPosY2[i] - 0.01 && bPosY1[i] - 0.01 < pPosY2) {


					//	//			//あったった面を記録
					//	//			if (pState == NorthPole && magnetBlocks[i].GetIsNorth() == 0) {
					//	//				magnetBlocks[i].SetContactNum(contact, i);
					//	//			}
					//	//			else if (pState == SouthPole && magnetBlocks[i].GetIsNorth() == 1) {
					//	//				magnetBlocks[i].SetContactNum(contact, i);
					//	//			}
					//	//			else {
					//	//				magnetBlocks[i].ReSetContactNum(contact);
					//	//			}

					//	//		}
					//	//	}

					//	//}

					//}
					 if (contact == 3) {
						//左に0.1移動してあったっていたら

						if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {

							if (pPosY1 < bPosY2[i] && bPosY1[i] < pPosY2) {


								if (pPosX1 < bPosX2[i] - 0.01 && bPosX1[i] - 0.01 < pPosX2) {
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
							}
						}
					}
					else if (contact == 4) {

						if (pPosZ1 < bPosZ2[i] && bPosZ1[i] < pPosZ2) {
							if (pPosY1 < bPosY2[i] && bPosY1[i] < pPosY2) {

								//右に0.1移動してあったっていたら
								if (pPosX1 < bPosX2[i] + 0.01 && bPosX1[i] + 0.01 < pPosX2) {

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
							}
						}

					}
					else if (contact == 5) {

						if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

							if (pPosY1 < bPosY2[i] && bPosY1[i] < pPosY2) {

								//上に0.1移動してあったっていたら

								if (pPosZ1 < bPosZ2[i] + 0.01 && bPosZ1[i] + 0.01 < pPosZ2) {

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
							}

						}

					}
					else if (contact == 6) {


						if (pPosX1 < bPosX2[i] && bPosX1[i] < pPosX2) {

							if (pPosY1 < bPosY2[i] && bPosY1[i] < pPosY2) {

								////下に0.1移動してあったっていたら

								if (pPosZ1 < bPosZ2[i] - 0.01 && bPosZ1[i] - 0.01 < pPosZ2) {

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
							}
						}
					}
				}
				//magnetBlocks[i].SetRockMove(true, contact);

			}
		}


		//自機を進めないように

		//if (pPosX1 + pMoveVec.x < bPosX2[i] && bPosX1[i] + pMoveVec.x < pPosX2) {

		//	if (pPosZ1 + pMoveVec.x < bPosZ2[i] && bPosZ1[i] - pMoveVec.z< pPosZ2) {


		//		if (pState == UnMagnet) {


		//			//自機が磁力なしの場合めり込ませないように
		//			if (contact == 1) {
		//				ColZ.y = 1;
		//			}
		//			else if (contact == 2) {
		//				ColZ.x = 1;
		//			}
		//			else if (contact == 3) {
		//				ColX.x = 1;
		//			}
		//			else if (contact == 4) {
		//				ColX.y = 1;
		//			}

		//		}
		//		else {

		//			//if (contact == 1) {
		//			//	if (magnetBlocks[i].GetRockMove(2)) {
		//			//		ColZ.y = 1;
		//			//	}
		//			//}
		//			//else if (contact == 2) {
		//			//	if (magnetBlocks[i].GetRockMove(1)) {
		//			//		ColZ.x = 1;
		//			//	}
		//			//}
		//			//else if (contact == 3) {
		//			//	if (magnetBlocks[i].GetRockMove(4)) {
		//			//		ColX.x = 1;
		//			//	}
		//			//}
		//			//else if (contact == 4) {
		//			//	if (magnetBlocks[i].GetRockMove(3)) {
		//			//		ColX.y = 1;
		//			//	}
		//			//}

		//		}

		//	}
		//}

		//ImGui::Begin("PlayerGetRock");
		//ImGui::Text("up = %f,down = %f,left = %f,righ = %f\n", ColZ.x, ColZ.y, ColX.y, ColX.x);
		//ImGui::End();

		//ImGui::Begin("contactP");
		//ImGui::Text("contact[%d] = %d \n", i, contact);
		//ImGui::End();

	}

	//pl
	// yer->SetColX(ColX);
	//player->SetColY(ColY);
	//player->SetColZ(ColZ);

	//for (int i = 0; i < magnetBlocks.size(); i++) {
	//	ImGui::Begin("rockMove");
	//	ImGui::Text("[%d]rockMove =up%d,down%d,left%d,right%d \n", i, magnetBlocks[i].GetRockMove(1), magnetBlocks[i].GetRockMove(2), magnetBlocks[i].GetRockMove(3), magnetBlocks[i].GetRockMove(4));
	//	//ImGui::Text("pPos = %f \n", pSize);
	//	ImGui::End();
	//}


	//----------磁石ブロックと磁石ブロック----------

	InforUpdateMagnetPos();

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

				if (bPosY1[j] < bPosY2[i] && bPosY1[i] < bPosY2[j]) {

					if (bPosZ1[j] < bPosZ2[i] && bPosZ1[i] < bPosZ2[j]) {

						//XMFLOAT3 pos1 = magnetBlocks[i].GetPos();
						//XMFLOAT3 pos2 = magnetBlocks[j].GetPos();

						//押し戻し処理
						while (true)
						{

							//ImGui::Begin("aa");
							//ImGui::Text("aaaa");
							//ImGui::End();

							//あたったのがx軸かz軸かで押し戻しを変更
							if ((magnetBlocks[i].Colision(pos1, pos1Size, pos2, pos2Size))) {

								//あたったのがx軸かz軸かで押し戻しを変更
								if (contact1 == 1 || contact1 == 2) {


									break;

									pos1.y -= adjust.y;

									bMoveVec[i].y -= adjust.y;

									pos2.y += adjust.y;

									bMoveVec[j].y += adjust.y;

								}
								else if (contact1 == 3 || contact1 == 4) {

									//adjustが+か-か
									bool adjustInteger;

									if (adjust.x >= 0) {
										adjustInteger = true;
									}
									else {
										adjustInteger = false;
									}

									if (magnetBlocks[i].GetRockMove(3) == false && adjustInteger == true) {
										pos1.x -= adjust.x;
										bMoveVec[i].x -= adjust.x;
									}
									else if (magnetBlocks[i].GetRockMove(4) == false && adjustInteger == false) {
										pos1.x -= adjust.x;
										bMoveVec[i].x -= adjust.x;
									}

									if (magnetBlocks[j].GetRockMove(3) == false && adjustInteger == false) {
										pos2.x += adjust.x;
										bMoveVec[j].x += adjust.x;
									}
									else if (magnetBlocks[j].GetRockMove(4) == false && adjustInteger == true) {
										pos2.x += adjust.x;
										bMoveVec[j].x += adjust.x;
									}


								}
								else if (contact1 == 5 || contact1 == 6) {

									pos1.z -= adjust.z;
									bMoveVec[i].z -= adjust.z;

									pos2.z += adjust.z;
									bMoveVec[j].z += adjust.z;

								}

							}
							else {

								//magnetBlocks[i].SetRockMove(true, contact1);
								//magnetBlocks[j].SetRockMove(true, contact2);

								break;
							}

						}

					}
				}
			}

		}
	}




	//自機と磁石座標変更で移動制限も変更

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ブロック 座標

		bPos[i].x = setPos[i].x + bMoveVec[i].x;
		bPos[i].y = setPos[i].y + bMoveVec[i].y;
		bPos[i].z = setPos[i].z + bMoveVec[i].z;


		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;

		bPosY1[i] = setPos[i].y - (bSize / 2) + bMoveVec[i].y;
		bPosY2[i] = setPos[i].y + (bSize / 2) + bMoveVec[i].y;

		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;

	}

	//InforUpdateMagnetPos()

	//あったった面を記録(Setcontact)
	// 
	//磁石と磁石

	for (int i = 0; i < magnetBlocks.size(); i++) {

		for (int j = 0; j < magnetBlocks.size(); j++) {


			if (j >= i) {
				break;
			}

			int contact1 = GetContact(bPos[j], bPos[i]);
			int contact2 = GetContact(bPos[i], bPos[j]);

			if (bPosZ1[j] - 0.01 < bPosZ2[i] + 0.01 && bPosZ1[i] - 0.01 < bPosZ2[j] + 0.01) {

				if (bPosX1[j] - 0.01 < bPosX2[i] + 0.01 && bPosX1[i] - 0.01 < bPosX2[j] + 0.01) {

					if (bPosY1[j] - 0.01 < bPosY2[i] + 0.01 && bPosY1[i] - 0.01 < bPosY2[j] + 0.01) {

						if (contact1 == 1) {

							if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

								//上に0.1移動してあったっていたら

								if (bPosZ1[j] < bPosZ2[i] + 0.01 && bPosZ1[i] + 0.01 < bPosZ2[j]) {

									magnetBlocks[i].SetContactNum(contact1, j);
									magnetBlocks[j].SetContactNum(contact2, i);

								}

							}

						}
						else if (contact1 == 2) {


							if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

								////下に0.1移動してあったっていたら

								if (bPosZ1[j] < bPosZ2[i] - 0.01 && bPosZ1[i] - 0.01 < bPosZ2[j]) {

									magnetBlocks[i].SetContactNum(contact1, j);
									magnetBlocks[j].SetContactNum(contact2, i);


								}
							}
						}
						else if (contact1 == 3) {
							//左に0.1移動してあったっていたら

							if (bPosZ1[j] < bPosZ2[i] && bPosZ1[i] < bPosZ2[j]) {


								if (bPosX1[j] < bPosX2[i] - 0.01 && bPosX1[i] - 0.01 < bPosX2[j]) {

									magnetBlocks[i].SetContactNum(contact1, j);
									magnetBlocks[j].SetContactNum(contact2, i);

								}

							}
						}
						else if (contact1 == 4) {

							if (bPosZ1[j] < bPosZ2[i] && bPosZ1[i] < bPosZ2[j]) {

								//右に0.1移動してあったっていたら
								if (bPosX1[j] < bPosX2[i] + 0.01 && bPosX1[i] + 0.01 < bPosX2[j]) {

									magnetBlocks[i].SetContactNum(contact1, j);
									magnetBlocks[j].SetContactNum(contact2, i);

								}

							}

						}
						else if (contact1 == 5) {

							if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

								//上に0.1移動してあったっていたら

								if (bPosZ1[j] < bPosZ2[i] + 0.01 && bPosZ1[i] + 0.01 < bPosZ2[j]) {

									magnetBlocks[i].SetContactNum(contact1, j);
									magnetBlocks[j].SetContactNum(contact2, i);

								}

							}

						}
						else if (contact1 == 6) {


							if (bPosX1[j] < bPosX2[i] && bPosX1[i] < bPosX2[j]) {

								////下に0.1移動してあったっていたら

								if (bPosZ1[j] < bPosZ2[i] - 0.01 && bPosZ1[i] - 0.01 < bPosZ2[j]) {

									magnetBlocks[i].SetContactNum(contact1, j);
									magnetBlocks[j].SetContactNum(contact2, i);


								}
							}
						}

					}
				}
			}

		}
	}

	//自機を進めないように
	InforUpdateMagnetPos();


	for (int i = 0; i < magnetBlocks.size(); i++) {

		int contact = GetContact(pPos, bPos[i]);

		float playerSpeed = player->GetSpeed();

		if (pPosX1 + pMoveVec.x < bPosX2[i] && bPosX1[i] < pPosX2 + pMoveVec.x) {

			if (pPosZ1 + pMoveVec.z < bPosZ2[i] && bPosZ1[i] < pPosZ2 + pMoveVec.z) {

				if (pPosY1 + pMoveVec.y < bPosY2[i] && bPosY1[i] < pPosY2 + pMoveVec.y) {


					if (pState == UnMagnet) {


						//自機が磁力なしの場合めり込ませないように
						if (contact == 1) {
							ColY.y = 1;
						}
						else if (contact == 2) {
							ColY.x = 1;
						}
						else if (contact == 3) {
							ColX.x = 1;
						}
						else if (contact == 4) {
							ColX.y = 1;
						}
						else if (contact == 5) {
							ColZ.y = 1;
						}
						else if (contact == 6) {
							ColZ.x = 1;
						}

					}
					else {

						if (contact == 1) {
							ColY.y = 1;
						}
						else if (contact == 2) {
							ColY.x = 1;

							ColX.x = 1;
						}

					}
				}

			}

		}
	}


	player->SetColX(ColX);
	player->SetColY(ColY);
	player->SetColZ(ColZ);
	//for (int i = 0; i < magnetBlocks.size(); i++) {
	//	ImGui::Begin("contact");
	//	ImGui::Text("[%d]contact =up%d,down%d,left%d,right%d \n", i, magnetBlocks[i].GetContactNum(1), magnetBlocks[i].GetContactNum(2), magnetBlocks[i].GetContactNum(3), magnetBlocks[i].GetContactNum(4));
	//	//ImGui::Text("pPos = %f \n", pSize);
	//	ImGui::End();
	//}


	for (int i = 0; i < magnetBlocks.size(); i++) {

		ImGui::Begin("contact");
		ImGui::Text("%d = %d \n", i, magnetBlocks[i].GetContactNum(3));
		ImGui::End();

	}
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

				//ImGui::Begin(" vecblockToblock");
				//ImGui::Text("[%d] = (%f,%f,%f) \n", i, vecMagToMag.x, vecMagToMag.y, vecMagToMag.z);
				//ImGui::End();

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


				//ImGui::Begin(" vecPlayerToblock");
				//ImGui::Text("[%d] = (%f,%f,%f) \n", i, vecPlayerToblock.x, vecPlayerToblock.y, vecPlayerToblock.z);
				//ImGui::End();


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
						//bMoveVec[i].y += moveVec.y;

						//if (magnetBlocks[i].GetRockMove(2) == false) {

						bMoveVec[i].z += moveVec.z;

						//}
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

						//if (magnetBlocks[i].GetRockMove(3) == false || magnetBlocks[i].GetRockMove(4) == false) {
						bMoveVec[i].x -= moveVec.x;
						//}

						//bMoveVec[i].y -= moveVec.y;

						//if (magnetBlocks[i].GetRockMove(1) == false || magnetBlocks[i].GetRockMove(2) == false) {
						bMoveVec[i].z -= moveVec.z;
						//}

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


	//float bSize = 2 * (magnetBlocks[0].GetSize()); // (2 * 0.99)

	//float pSize = 2 * player->GetSize();

	int isPower[20] = {};

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ImGui::Begin("conatct");
		//ImGui::Text("Mag[%d] = (Up = %d ,Down = %d,left = %d,right = %d,ZUp = %d,ZDown = %d) \n", i, magnetBlocks[i].GetContactNum(1), magnetBlocks[i].GetContactNum(2), magnetBlocks[i].GetContactNum(3), magnetBlocks[i].GetContactNum(4), magnetBlocks[i].GetContactNum(5), magnetBlocks[i].GetContactNum(6));
		//ImGui::End();
	}

	//配列の最大数-1回for文を回す
	for (int i = 0; i < magnetBlocks.size() - 1; i++) {

		//i個目の磁石に対して、i+1 ~ 配列末尾までのブロックと磁石の判定を行う
		for (int j = i + 1; j < magnetBlocks.size(); j++) {

			for (int k = 1; k < 7; k++) {

				////自機と磁石

				if (magnetBlocks[i].GetContactNum(k) == i) {

					/*			debugText_->SetPos(0, 80);
								debugText_->Printf("qqqqqqqqqqqq ");*/

					if (k == 1) {
						if (magnetBlocks[i].GetPos().y - bSize / 2 + bMoveVec[i].y <= pPos.y + bMoveVec[j].y) {
							magnetBlocks[i].SetIsMove(false);

						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}

					if (k == 2) {
						if (magnetBlocks[i].GetPos().y + bSize / 2 + bMoveVec[i].y > pPos.y + bMoveVec[j].y) {
							magnetBlocks[i].SetIsMove(false);
						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}


					if (k == 3) {
						if (magnetBlocks[i].GetPos().x - bSize / 2 + bMoveVec[i].x >= pPos.x + bMoveVec[j].x) {
							magnetBlocks[i].SetIsMove(false);

						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}


					if (k == 4) {
						if (magnetBlocks[i].GetPos().x + bSize / 2 + bMoveVec[i].x < pPos.x + bMoveVec[j].x) {
							magnetBlocks[i].SetIsMove(false);
						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}

					if (k == 5) {
						if (magnetBlocks[i].GetPos().z + bSize / 2 + bMoveVec[i].z >= pPos.z + bMoveVec[j].z) {
							magnetBlocks[i].SetIsMove(false);

						}
						else {
							magnetBlocks[i].SetIsMove(true);
						}
					}

					if (k == 6) {
						if (magnetBlocks[i].GetPos().z - bSize / 2 + bMoveVec[i].z < pPos.z + bMoveVec[j].z) {
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
							//if (setPos[i].y + (bSize / 2) + bMoveVec[i].y < setPos[j].y + bMoveVec[j].y) {
							//	magnetBlocks[i].SetIsMagMove(j, false);
							//	isPower[i] = 1;
							//}
							//else {
							//	if (isPower[i] == 0) {
							//		magnetBlocks[i].SetIsMagMove(j, true);
							//	}
							//}
						}

						else if (k == 2) {
							//if (setPos[i].y - (bSize / 2) + bMoveVec[i].y > setPos[j].y + bMoveVec[j].y) {
							//	magnetBlocks[i].SetIsMagMove(j, false);
							//	isPower[i] = 1;

							//}
							//else {
							//	if (isPower[i] == 0) {
							//		magnetBlocks[i].SetIsMagMove(j, true);
							//	}
							//}
						}


						else if (k == 3) {
							if (setPos[i].x - bSize / 2 + bMoveVec[i].x >= setPos[j].x + bMoveVec[j].x) {
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
							if (setPos[i].x + bSize / 2 + bMoveVec[i].x < setPos[j].x + bMoveVec[j].x) {
								magnetBlocks[i].SetIsMagMove(j, false);
								isPower[i] = 1;


							}
							else {
								if (isPower[i] == 0) {
									magnetBlocks[i].SetIsMagMove(j, true);
								}
							}
						}
						else if (k == 5) {
							if (setPos[i].z + bSize / 2 + bMoveVec[i].z < setPos[j].z + bMoveVec[j].z) {
								magnetBlocks[i].SetIsMagMove(j, false);
								isPower[i] = 1;
							}
							else {
								if (isPower[i] == 0) {
									magnetBlocks[i].SetIsMagMove(j, true);
								}
							}
						}

						else if (k == 6) {
							if (setPos[i].z - bSize / 2 + bMoveVec[i].z > setPos[j].z + bMoveVec[j].z) {
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

	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	ImGui::Begin("MagPower");
	//	ImGui::Text("MagPower[%d] = (0 = %d ,1 = %d,2 = %d,3 = %d) \n", i, magnetBlocks[i].GetIsMagMove(0), magnetBlocks[i].GetIsMagMove(1), magnetBlocks[i].GetIsMagMove(2), magnetBlocks[i].GetIsMagMove(3));
	//	ImGui::End();
	//}

}

void Colision::InforUpdate()
{

	//--------自機----------

	pPos = player->GetPosition();

	pSize = player->GetSize();
	pState = player->GetState();

	pPosX1 = pPos.x - (pSize / 2);
	pPosX2 = pPos.x + (pSize / 2);

	pPosY1 = pPos.y - (pSize / 2);
	pPosY2 = pPos.y + (pSize / 2);

	pPosZ1 = pPos.z - (pSize / 2);
	pPosZ2 = pPos.z + (pSize / 2);

	pMoveVec = player->GetMove();


	//当たっているか
	ColX = { 0,0 };
	ColY = { 0,0 };
	ColZ = { 0,0 };

	//nブロック 座標

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ブロック 座標

		bPos[i] = magnetBlocks[i].GetPos();

		bPosX1[i] = bPos[i].x - (bSize / 2);
		bPosX2[i] = bPos[i].x + (bSize / 2);

		bPosY1[i] = bPos[i].y - (bSize / 2);
		bPosY2[i] = bPos[i].y + (bSize / 2);

		bPosZ1[i] = bPos[i].z - (bSize / 2);
		bPosZ2[i] = bPos[i].z + (bSize / 2);

		setPos[i] = bPos[i];

		for (int j = 0; j < 7; j++) {
			magnetBlocks[i].ReSetContactNum(j);
			magnetBlocks[i].SetRockMove(false, j);
		}

		bMoveVec[i] = { 0,0,0 };

	}
	bSize = magnetBlocks[0].GetSize();

}

void Colision::StickMag()
{

	XMFLOAT3 massMoveVec = {};

	int massRockMove[5] = {};

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (stickBlockMass[i]) {

			massMoveVec.x += bMoveVec[i].x;
			//massMoveVec.y += bMoveVec[i].y;
			massMoveVec.z += bMoveVec[i].z;

			//for (int j = 1; j < 5; j++) {

			//	if (magnetBlocks[i].GetRockMove(j)) {
			//		massRockMove[j] = true;
			//	}

			//}

		}

	}

	for (int i = 0; i < magnetBlocks.size(); i++) {

		if (stickBlockMass[i]) {

			bMoveVec[i] = massMoveVec;

		}

		//for (int j = 1; j < 5; j++) {

		//	if (massRockMove[j]) {
		//		magnetBlocks[i].SetRockMove(true, j);
		//	}

		//}

	}


	//for (int i = 0; i < magnetBlocks.size(); i++) {

	//	ImGui::Begin("BlockGetRockAfter");
	//	ImGui::Text("up = %d,down = %d,left = %d,righ = %d\n", magnetBlocks[i].GetRockMove(1), magnetBlocks[i].GetRockMove(2), magnetBlocks[i].GetRockMove(3), magnetBlocks[i].GetRockMove(4));
	//	ImGui::End();
	//}

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

void Colision::SetStickMag()
{


	//くっついているブロックをtrueに
	for (int i = 0; i < magnetBlocks.size(); i++) {


		//stickBlockMass[i] = false;

		for (int k = 1; k < 7; k++) {

			//もしくっついているなら
			if (magnetBlocks[i].GetContactNum(k) != 100 && magnetBlocks[i].GetContactNum(k) != i) {

				if (k == 1 || k == 2) {


					float diff = (setPos[i].y + bMoveVec[i].y) - (setPos[magnetBlocks[i].GetContactNum(k)].y + bMoveVec[magnetBlocks[i].GetContactNum(k)].y);

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


				if (k == 5 || k == 6) {


					float diff = (setPos[i].x + bMoveVec[i].x) - (setPos[magnetBlocks[i].GetContactNum(k)].x + bMoveVec[magnetBlocks[i].GetContactNum(k)].x);

					//完全にくっついたらくっついたをtrueに
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

	//落下中のブロックをfalseに
	for (int i = 0; i < magnetBlocks.size(); i++) {
		if (bFall[i]) {
			stickBlockMass[i] = false;
		}
	}

}

void Colision::InforUpdateMagnetPos()
{
	for (int i = 0; i < magnetBlocks.size(); i++) {

		//ブロック 座標


		bPos[i] = setPos[i];


		bPosX1[i] = setPos[i].x - (bSize / 2) + bMoveVec[i].x;
		bPosX2[i] = setPos[i].x + (bSize / 2) + bMoveVec[i].x;

		bPosZ1[i] = setPos[i].z - (bSize / 2) + bMoveVec[i].z;
		bPosZ2[i] = setPos[i].z + (bSize / 2) + bMoveVec[i].z;

		bPosY1[i] = setPos[i].y - (bSize / 2) + bMoveVec[i].y;
		bPosY2[i] = setPos[i].y + (bSize / 2) + bMoveVec[i].y;

	}
}

void Colision::MagFall()
{

	for (int i = 0; i < magnetBlocks.size(); i++) {

		//落下関係

		float leftmagnetBlocks = magnetBlocks[i].GetPos().x;
		float downmagnetBlocks = magnetBlocks[i].GetPos().y;
		float frontmagnetBlocks = magnetBlocks[i].GetPos().z;

		float rightmagnetBlocks = magnetBlocks[i].GetPos().x + magnetBlocks[i].GetSize();
		float upmagnetBlocks = magnetBlocks[i].GetPos().y + magnetBlocks[i].GetSize();
		float backmagnetBlocks = magnetBlocks[i].GetPos().z + magnetBlocks[i].GetSize();

		float magnetBlocksSpeed = 0.05;

		//当たらないよう調整する用
		float adjustPixcelSpeed = 0.01;
		//サイズ調整用
		float adjustmagnetBlocksSize = 0.00;

		//下に仮想的に移動して当たったら
		if (map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks - magnetBlocksSpeed, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks - magnetBlocksSpeed, backmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks, downmagnetBlocks - magnetBlocksSpeed, frontmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks, downmagnetBlocks - magnetBlocksSpeed, backmagnetBlocks))
		{
			bFall[i] = false;

		}
		else {

			bFall[i] = true;
			//magnetBlocks[i].SetIsMove(false);

		}

		//上に仮想的に移動して当たったら
		if (map_->mapInGoalCol(leftmagnetBlocks, upmagnetBlocks + magnetBlocksSpeed, frontmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks, upmagnetBlocks + magnetBlocksSpeed, backmagnetBlocks) || map_->mapInGoalCol(rightmagnetBlocks, upmagnetBlocks + magnetBlocksSpeed, frontmagnetBlocks) || map_->mapInGoalCol(leftmagnetBlocks, upmagnetBlocks + magnetBlocksSpeed, backmagnetBlocks))
		{
			//magnetBlocks->SetFall(true);
			//magnetBlocks->SetJump(false);

		}
		else {

			//magnetBlocks->SetFall(false);

		}

		for (int i = 0; i < magnetBlocks.size(); i++) {

			if (bFall[i] == true /*&& bColY.y == false*/) {


				bMoveVec[i].y -= 0.025;
			}

		}
	}

}

//void Colision::MagnetBlocksFall()
//{
//}

int Colision::GetContact(XMFLOAT3 mainPos, XMFLOAT3 subPos)
{
	//0なし 1上　2下　3左　4右 5奥 6手前
	int contact = 0;

	//X軸 Y軸 Z軸判断用
	float contactNumX = 0;
	float contactNumY = 0;
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
			contact = 5;
		}
		else {
			contact = 6;
		}

	}

	if (mainPos.y > subPos.y) {
		contactNumY = mainPos.y - subPos.y/* + 0.05*/;
	}
	else {
		contactNumY = subPos.y - mainPos.y/* + 0.05*/;
	}

	if (contactNumX < contactNumZ) {

		if (contactNumZ < contactNumY) {

			if (mainPos.y > subPos.y) {
				contact = 1;
			}
			else {
				contact = 2;
			}

		}

	}
	else {

		if (contactNumX < contactNumY) {

			if (mainPos.y > subPos.y) {
				contact = 1;
			}
			else {
				contact = 2;
			}

		}
	}


	return contact;
}

void Colision::ReSet()
{



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