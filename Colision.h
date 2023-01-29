#pragma once

#include"Player.h"
#include "Map.h"
#include"MagnetBlock.h"
#include <DirectXMath.h>

using namespace DirectX;

class Colision
{

public:
	/// <summary>
/// �R���X�g�N���^
/// </summary>
	Colision();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Colision();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Player* player, MagnetBlock magnetBlock, int i);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	void UpdateDeta(Player* player, MagnetBlock magnetBlock, int i);

	//�����蔻��
//�}�b�v�Q��
	void MapCollision();

	//���W�Q��
	void PosCollision();

	//���΂̈����񂹁A���������̍X�V����
	void MagnetsUpdate();

	//���Γ��m�̈����񂹁A���������̍X�V����
	void MagToMagUpdate();

	//���΂ƃv���C���[�̈����񂹁A���������̍X�V����
	void MagToPlayerUpdate();

	//���͂�ON,OFF
	void MagnetPower();

	//���W�Ȃǂ̏����X�V
	void InforUpdate();

	//���������u���b�N�̈ړ��x�N�g���𓝈ꂳ����
	void StickMag();

	//mainPos �̂ǂ̖ʂ�subPos�ɂ���������
	int GetContact(XMFLOAT3 mainPos, XMFLOAT3 subPos);

	//pos2 - pos1�@�� pos1�@���� pos2�@�̃x�N�g�������߂�
	XMFLOAT3 GetVec(XMFLOAT3 pos1, XMFLOAT3 pos2);

	//�x�N�g���𐳋K������size��������֐�
	XMFLOAT3 ChangeVec(XMFLOAT3 changeVec, float size);

	float lengthVec(XMFLOAT3 vec);

	MagnetBlock GetMagnetBlocks(int i) { return magnetBlocks[i]; }

	std::vector<MagnetBlock> magnetBlocks;

	XMFLOAT3 setPos[20] = {};

public: // �����o�ϐ�

	Player* player = nullptr;

	Map* savemap_ = new Map;

	//std::vector<MagnetData> magnetDatas;

	XMFLOAT3 vecMagToMag;

	//���@�������Ă��邩
	XMFLOAT2 ColX = { 0,0 };
	XMFLOAT2 ColY = { 0,0 };
	XMFLOAT2 ColZ = { 0,0 };

	//�������Ă���u���b�N�̔ԍ�
	bool stickBlockMass[20] = { 0,0,0,0 };

	//�������Ă���u���b�N�̒��łǂꂪ1�ԃv���C���[�ɋ߂���
	int mostNearPlayerBlock = 0;
	bool mostNear;

	//�����4�ʂ̂�
	int mostNearPlayerBlockFour = 0;
	bool mostNearFour;

	int a;

	//�u���b�N�̍ő吔
	static const int blockSizeMax = 20;

	//n�u���b�N ���W

	XMFLOAT3 bPos[blockSizeMax];

	float bPosX1[blockSizeMax] = {};
	float bPosX2[blockSizeMax] = {};

	float bPosZ1[blockSizeMax] = {};
	float bPosZ2[blockSizeMax] = {};

	//�u���b�N�̓����蔻��̑傫��
	float bSize = 0.99f; // (2 * 0.99)


	//XMFLOAT3 setPos[blockSizeMax] = {};

	//XMFLOAT3 bMoveVec;


	XMFLOAT3 bMoveVec[blockSizeMax] = {};

	//XMFLOAT3 moveVec2 = { };


	//--------���@----------

	XMFLOAT3 pPos;
	float pSize;
	int pState;

	float pPosX1;
	float pPosX2;

	float pPosZ1;
	float pPosZ2;

	XMFLOAT3 pMoveVec;

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>

};

