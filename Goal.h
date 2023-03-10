#pragma once
#include"Object3d.h"
#include "Sprite.h"
#include "Input.h"
#include "AudioManager.h"

using namespace DirectX;

class Goal
{
private:
	XMFLOAT3 pos;
	Input* input = nullptr;

public:
	void Initialize(Input* input, const uint32_t& tex,XMFLOAT3 pos);
	void Update();
	void Draw();

	//ゴールした時の処理
	void InGoal();

	XMFLOAT3 GetPos() { return pos; }
	void SetPos(XMFLOAT3 setPos) { this->pos = setPos; }

public:

	AudioManager* goalSE;
	float goalSEVolume = 1.0f;

	Object3d obj;

	uint32_t goalTexture = 0;

	bool isGoal = false;

	int stage = 0;
	int scene = 0;

};

