#include "Map.h"
#include <cassert>

bool Map::mapcol(int x, int y, int z)
{
	int mapx = x / (blockScale * blockSize);
	int mapy = y / (blockScale * blockSize);
	int mapz = z / (blockScale * blockSize);

	//���肦�Ȃ��l��false�ŕԂ�
	if (mapx < 0 || mapx >= blockX)	return false;
	if (mapy < 0 || mapy >= blockY)	return false;
	if (mapz < 0 || mapz >= blockZ)	return false;

	return Map::map[mapy][mapz][mapx] == BLOCK || Map::map[mapy][mapz][mapx] == GOAL;
}

bool Map::mapGoalCol(int x, int y, int z)
{
	int mapx = x / (blockScale * blockSize);
	int mapy = y / (blockScale * blockSize);
	int mapz = z / (blockScale * blockSize);

	//���肦�Ȃ��l��false�ŕԂ�
	if (mapx < 0 || mapx >= blockX)	return false;
	if (mapy < 0 || mapy >= blockY)	return false;
	if (mapz < 0 || mapz >= blockZ)	return false;

	return Map::map[mapy][mapz][mapx] == GOAL;
}

void Map::SetSize(Vector3 stageSize)
{

	blockX = (int)stageSize.x;
	blockY = (int)stageSize.y;
	blockZ = (int)stageSize.z;


}

void Map::Initialize() {
}

void Map::Loding(char const* _FileName)
{
	FILE* fp = NULL;

	fopen_s(&fp, _FileName, "r");

	//csvからマップ情報を書き写す
	int i, j, k;
	i = 0; j = 0; k = 0;
	//���
	while (i < blockY && j < blockZ && k < blockX) {
		fscanf_s(fp, "%d,", &map[i][j][k]);
		if (k < blockX - 1)
		{
			k++;
		}
		else if (j < blockZ - 1)
		{
			k = 0;
			j++;
		}
		else if (i < blockY - 1)
		{
			k = 0;
			j = 0;
			i++;
		}
		else {
			break;
		}

	}


	fclose(fp);
}

void Map::LodingSave(int stage)
{
	if (stage == 1)
	{
		Loding("map/stage1.csv");
	}
	if (stage == 2)
	{
		Loding("map/tutorialHole.csv");
	}
	if (stage == 3)
	{
		Loding("map/stage1.csv");
	}
	if (stage == 4)
	{
		Loding("map/stage2.csv");
	}
	if (stage == 5)
	{
		Loding("map/stage3.csv");
	}
	if (stage == 6)
	{
		Loding("map/stage4.csv");
	}
	if (stage == 7)
	{
		Loding("map/stage5.csv");
	}
	if (stage == 8)
	{
		Loding("map/stage6.csv");
	}
	if (stage == 9)
	{
		Loding("map/stage7.csv");
	}
	if (stage == 10)
	{
		Loding("map/stage8.csv");
	}
}

void Map::Update(int stage) {

}
