#pragma once
#include"DirectX.h"
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include "Vector3.h"

class Camera
{
public:
	static ID3D12Device* device;

	static void StaticInitialize(ID3D12Device* dev);

	struct ConstBufferCamera {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	ConstBufferCamera* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matProjection;
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;

public:


	void Initialize(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	void UpdateMatrix();

	DirectX::XMMATRIX GetViewProjection()const { return matView * matProjection; }

	//マップの大きさを受け取る
	void InitializeData(Vector3 stageSiz);

	//eyeの更新
	void UpdateEye();

	//自機追従,上下左右
	void ChangeEye(int place);

	//ベジエ曲線での移動
	void MoveEye();

private:

	//0自機追従,1上2下3左4右
	int state = 0;
	int nextState;

	//場所ごとの座標
	Vector3 placePos[5];

	//+何足すか
	float addSizeX = 10;
	float addSizeY = 10;
	float addSizeZ = 10;


	//↓------ベジエ曲線-------↓

//時間計測に必要なデータ
	long long elapsedCount = 0;

	//補間で使うデータ
	//start -> end　を [s] で完了させる
	Vector3 start;	//スタート地点
	Vector3 p;	//制御点
	Vector3 end;	//エンド地点

	float maxTime = 2.0f;		//全体時間[s]
	float timeRate;				//何％時間が進んだか(率)

	Vector3 cameraPosition;
	//移動中か
	bool isMove = false;

};

