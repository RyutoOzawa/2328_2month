#include "Goal.h"

void Goal::Initialize(Input* input,const uint32_t& tex,XMFLOAT3 pos)
{
	this->pos = pos;
	goalTexture = tex;

	//オブジェクトデータ初期化
	obj.Initialize();
	obj.model = Model::CreateModel("goalObj");

	obj.position = pos;
	//obj.rotation.x = XM_PI / 2.0f;

	//プレイヤーのデフォルトカラーは白

	this->input = input;

}

void Goal::Update()
{

	obj.rotation.y += 4.0f * DirectX::XM_PI / 180;
	if (obj.rotation.y > XM_2PI)obj.rotation.y -= XM_2PI;

	obj.Update();

	if (isGoal) {

		if (input->IsKeyPress(DIK_N)) {
			stage++;
			isGoal = false;
		}
		else if (input->IsKeyPress(DIK_M)) {
			scene = 1;
			isGoal = false;
		}

	}

}

void Goal::Draw()
{
//	obj.model->textureIndex = goalTexture;
	obj.Draw();

}

void Goal::InGoal()
{

	isGoal = true;

}
