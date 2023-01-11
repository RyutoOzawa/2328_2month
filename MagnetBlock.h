#pragma once
#include"Object3d.h"

class MagnetBlock
{

public:
	void Initialize(const DirectX::XMFLOAT3& pos, bool isNorth_);

	void Update(const DirectX::XMFLOAT3& playerPos, int playerState, float moveDistance,DirectX::XMMATRIX& matView,DirectX::XMMATRIX& matProjection);

	void Draw(const uint32_t& nPoleTexuture, const uint32_t& sPoleTexture);

	DirectX::XMFLOAT3 GetPos() { return pos; };
	void SetPos(DirectX::XMFLOAT3 pos) { this->pos = pos; };

	void SetMove(int i) { this->move = i; }
	DirectX::XMFLOAT3 GetMoveVec() { return moveVec; }

	bool GetIsNorth() const { return isNorth; }

	float GetMoveSpd()const { return moveSpd; }


private:
	Object3d* obj = nullptr;
	DirectX::XMFLOAT3 pos{};
	uint32_t textureHandle = 0;
	DirectX::XMFLOAT3 moveVec;

	bool isNorth = false;
	float moveSpd = 0.025f;

	bool move = 1;

};

