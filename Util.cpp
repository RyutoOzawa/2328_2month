#include"Util.h"
#include<random>
#include<time.h>
#include<cmath>
using namespace std;

float Random(float num1, float num2) {

	float min, max;

	min = num2;
	max = num1;

	if (num1 < num2) {
		min = num1;
		max = num2;
	}



	//乱数シード生成器
	std::random_device seedGem;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seedGem());
	//乱数範囲の指定
	std::uniform_real_distribution<float> dist(min, max);

	return dist(engine);
}

DirectX::XMFLOAT2 EaseIn2D(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end, float timer)
{
	float nowTime;

	//ここにイージングの式を入れる
	nowTime = pow(timer, 3);

	DirectX::XMFLOAT2 resultVec;
	resultVec.x = start.x * (1.0f - nowTime) + end.x * nowTime;
	resultVec.y = start.y * (1.0f - nowTime) + end.y * nowTime;

	return resultVec;
}

DirectX::XMFLOAT2 EaseOut2D(DirectX::XMFLOAT2 start, DirectX::XMFLOAT2 end, float timer)
{
	float nowTime;

	//ここにイージングの式を入れる
	nowTime = 1 - pow(1 - timer, 3);

	DirectX::XMFLOAT2 resultVec;
	resultVec.x = start.x * (1.0f - nowTime) + end.x * nowTime;
	resultVec.y = start.y * (1.0f - nowTime) + end.y * nowTime;

	return resultVec;
}

void EaseingData::Update()
{
	//現在時刻を取得
	nowCount = clock();
	//取得されたイージング開始時間で終了時間が決まる
	endCount = nowCount - startCount;
	//タイマーの進み具合をfloat型に変換
	endTimer = static_cast<float>(endCount) / 1'000.0f;
	//(タイマーの進行度/最大時間)でイージングの進み具合を計算
	timeRate = min(endTimer / maxTime, 1.0f);
	//timeRate = endTimer / maxTime;
}

void EaseingData::Start(float animationSecond)
{
	startCount = clock();
	maxTime = animationSecond;
}