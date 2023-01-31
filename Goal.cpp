#include "Goal.h"

void Goal::Initialize(Input* input,const uint32_t& tex,XMFLOAT3 pos)
{
	this->pos = pos;
	goalTexture = tex;

	//オブジェクトデータ初期化
	obj.Initialize();
	obj.model = Model::CreateModel();

	obj.position = pos;
	//プレイヤーのデフォルトカラーは白

	this->input = input;

}

void Goal::Update()
{
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
	obj.model->textureIndex = goalTexture;
	obj.Draw();

}

void Goal::InGoal()
{

	isGoal = true;

}
