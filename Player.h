#pragma once
#include"Object3d.h"
#include"Input.h"
#include "Map.h"
#include "Goal.h"

enum stateIndex {
	UnMagnet,
	NorthPole,
	SouthPole,
};

class Player
{
public:
	void Initialize(const uint32_t& texW, const uint32_t& texR,
		const uint32_t& texB, Input* input, Map* map, Goal* goal);

	void Update();

	void Draw();

	void Fall();

	void Jump();

	void GoalCol();

	//当たり判定用 2はマイナス
	void OnMapCollision();
	void OnMapCollisionX();
	void OnMapCollisionY();
	void OnMapCollisionZ();
	void OnMapCollisionX2();
	void OnMapCollisionY2();
	void OnMapCollisionZ2();

	//ゲッターまとめ
	DirectX::XMFLOAT3 GetTranslation()const { return obj.position; }
	DirectX::XMFLOAT3 GetPosition()const { return pos; }
	DirectX::XMFLOAT3 GetMove()const { return move; }
	float GetSize()const { return size; }
	float GetSpeed()const { return playerSpd; }
	float GetAdjustPixelSpeed()const { return adjustPixelSpeed; }
	int GetState()const { return state; }

	bool GetIsGoal() {return goal->isGoal;}

	//セッター
	void SetColX(DirectX::XMFLOAT2 colX_) { colX = colX_; }
	void SetColY(DirectX::XMFLOAT2 colY_) { colY = colY_; }
	void SetColZ(DirectX::XMFLOAT2 colZ_) { colZ = colZ_; }

	void SetFall(bool isfall) { fall = isfall; }
	bool GetFall() { return fall; }

	void SetJump(bool isJump) { this->isJump = isJump; }

	void SetPosition(DirectX::XMFLOAT3 pos_) { pos = pos_; }

public:

	Object3d obj;

	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0, 2, 0);
	DirectX::XMFLOAT3 move = { 0,0,0 };

	float playerSpd = 0.05f;
	float adjustPixelSpeed = 0.001f;


	float size = 0.98;

	DirectX::XMFLOAT2 colX{};
	DirectX::XMFLOAT2 colY{};
	DirectX::XMFLOAT2 colZ{};

	//プライベートメンバ関数
private:
	void Move();
	void ChangeState();


	//プライベートメンバ変数
private:

	Input* input = nullptr;

	uint32_t whiteTexture = 0;
	uint32_t blueTexture = 0;
	uint32_t redTexture = 0;
	uint32_t playerTexture = 0;

	Map* map = nullptr;

	bool fall = false;

	bool isJump = false;

	float jumpHight = 4.0f;

	float jumpBeforePosY;

	Goal *goal = nullptr;

	int state =UnMagnet;
};


