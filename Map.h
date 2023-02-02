#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <array>
#include <d3dx12.h>
#include <unordered_map>
#include <wrl.h>
#include "Vector3.h"

//ブロックのサイズ 変更不可
const float blockSize = 10.0f;
//ブロックのサイズ 変更可
const float blockScale = 0.1f;

//xyzのブロックの数

//const int blockX = 20;
//const int blockY = 5;
//const int blockZ = 20;

enum mapdate
{
	BLUNK,
	BLOCK,
	NMAG,
	SMAG,
	PLAYER,
	GOAL
};

class Map
{
public:

	void Initialize();

	void Update(int stage);

	////ステージのマップをセットする
	//void SetMap(int stage);

	////他の場所でステージ情報を取得する
	//int GetMap(int x,int y, int z);

	void Loding(char const* _FileName);
	//どのマップを読み込むかを管理
	void LodingSave(int stage);


	bool mapcol(int x, int y, int z);

	bool mapGoalCol(int x, int y, int z);


	int dist(int x, int y, int z);

	void SetSize(Vector3 stageSize);

public:

	//static const int blockX = 20;
	//static const int blockY = 5;
	//static const int blockZ = 20;

	//int map[blockY][blockZ][blockX];
	//int savemap[blockY][blockZ][blockX];

	int blockX = 20;
	int blockY = 5;
	int blockZ = 20;

	int map[20][20][20];
	int savemap[20][20][20];

};