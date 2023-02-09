#pragma once
//#include"WorldTransform.h"
#include"Object3d.h"
//#include"DebugText.h"

#include <DirectXMath.h>
using namespace DirectX;

struct MagnetData
{
	XMFLOAT3 pos;
	bool isNorth_;
};

class MagnetBlock
{
public:
	~MagnetBlock();

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	/// <param name="pos"></param>
	void Initialize(MagnetData magnetData);

	void Update();

	void Draw(/*const ViewProjection& viewProjection,*/ const uint32_t& nPoleTexture, const uint32_t& sPoleTexture);

	XMFLOAT3 GetPos() { return pos; };
	void SetPos(XMFLOAT3 pos) { this->pos = pos; };

	//XMFLOAT3 GetTenPos() { return tentativePos; };
	//void SetTenPos(XMFLOAT3 tentativePos) { this->tentativePos = tentativePos; };
	//void AddTenPos(XMFLOAT3 pos);
	//void SubTenPos(XMFLOAT3 pos);


	//自機との磁力ON,OFF
	void SetIsMove(bool isMove) { this->isMove = isMove; }
	bool GetIsMove() { return isMove; }

	//磁石との磁力ON,OFF
	void SetIsMagMove(int num, bool isMagMove) { this->isMagMove[num] = isMagMove; }
	bool GetIsMagMove(int num) { return isMagMove[num]; }

	//N極かS極か
	bool GetIsNorth() const { return isNorth; }

	//移動するスピード
	float GetMoveSpd()const { return moveSpd; }

	//当たっていたらtrueを返す
	bool Colision(XMFLOAT3 pos1, float pos1Size, XMFLOAT3 pos2, float pos2Size);

	float GetSize()const { return size; }

	//numにどの面か　contactNumにあったったブロック
	void SetContactNum(int num, int contactNum) { this->contactNum[num] = contactNum; }
	int GetContactNum(int num) { return  contactNum[num]; }
	void ReSetContactNum(int num) { this->contactNum[num] = 100; }

	//進めない方向 第一引数に 進めなくする = true 進める = false 第二引数にどの面か
	void SetRockMove(bool rock, int num) { this->rockMove[num] = rock; }
	bool GetRockMove(int num) { return this->rockMove[num]; }

public:
	Object3d obj;

private:

	//DebugText* debugText = nullptr;
	//WorldTransform worldTransform;

	XMFLOAT3 pos{};
	////仮座標
	//XMFLOAT3 tentativePos{};
	uint32_t textureHandle = 0;
	XMFLOAT3 moveVec;

	bool isNorth = false;
	float moveSpd = 0.025f;

	//引き寄せ処理ON OFF
	//プレイヤーとの
	bool isMove = true;
	//磁石との
	bool isMagMove[20] = {};

	float size = 0.9;

	//　Y上下 X左右 Z奥手前
	int contactNum[7] = {};

	//進めない方向
	int rockMove[7] = {};

};