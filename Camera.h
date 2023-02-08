#pragma once
#include"DirectX.h"
#include<DirectXMath.h>
#include<d3d12.h>
#include<wrl.h>
#include "Vector3.h"

using namespace DirectX;

class Camera
{
public:
	static ID3D12Device* device;

	static void StaticInitialize(ID3D12Device* dev);

	struct ConstBufferCamera {
		XMMATRIX view;
		XMMATRIX projection;
	};

	ConstBufferCamera* constMap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	XMMATRIX matView;
	XMMATRIX matProjection;
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

public:


	void Initialize(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up);

	void UpdateMatrix();

	XMMATRIX GetViewProjection()const { return matView * matProjection; }

	//マップの大きさを受け取る
	void InitializeData(Vector3 stageSiz);

	void Update(XMFLOAT3 pPos);

	//eyeの更新
	void UpdateEye();

	//targetの更新
	void UpdateTarget(XMFLOAT3 pPos);

	//自機追従,上下左右
	void ChangeState(int state);

	//ベジエ曲線での移動
	void MoveEye();

	//カメラと中心点のベクトル

	XMFLOAT3 GetCameraVec() { return cameraVec; }

	void UpdateCameraVec();

	bool GetisMoveEye() { return isMoveEye; }

private:

	//0自機追従,1上2下3左4右
	int state_ = 0;
	int nextState;

	//場所ごとの座標
	Vector3 stagePos[5];

	//ステージの中心
	Vector3 stageMid;

	//+何足すか
	float addSizeX = 10;
	float addSizeY = 10;
	float addSizeZ = 10;

	//カメラと中心点のベクトル
	XMFLOAT3 cameraVec;

	//↓------ベジエ曲線-------↓


	float maxTime = 2.0f;		//全体時間[s]
	float timeRate;				//何％時間が進んだか(率)

	//-----eye-----

	//時間計測に必要なデータ
	long long elapsedCountEye = 0;

	//補間で使うデータ
	//start -> end　を [s] で完了させる
	Vector3 startEye;	//スタート地点
	Vector3 pEye;	//制御点
	Vector3 endEye;	//エンド地点

	Vector3 cameraPosition;

	//移動中か
	bool isMoveEye = false;

};

