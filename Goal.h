#pragma once
#include"Object3d.h"
#include "Sprite.h"
#include "Input.h"
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

	//ƒS[ƒ‹‚µ‚½Žž‚Ìˆ—
	void InGoal();

	XMFLOAT3 GetPos() { return pos; }
	void SetPos(XMFLOAT3 setPos) { this->pos = setPos; }

public:

	Object3d obj;

	uint32_t goalTexture = 0;

	bool isGoal = false;

	int stage = 0;
	int scene = 0;

};

