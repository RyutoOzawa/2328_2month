#include "Camera.h"
#include "Camera.h"
#include"WindowsAPI.h"
using namespace DirectX;

ID3D12Device* Camera::device = nullptr;

void Camera::StaticInitialize(ID3D12Device* dev)
{
	//NULLチェック
	assert(dev);
	device = dev;
}

void Camera::Initialize(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up)
{
	//引数をメンバに移して行列更新
	this->eye = eye;
	this->target = target;
	this->up = up;



	HRESULT result;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{  };
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferCamera) + 0xff) & ~0xff;	//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
	assert(SUCCEEDED(result));

	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	//専用の行列を宣言
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),					//上下画角45度
		(float)WindowsAPI::winW / WindowsAPI::winH,	//アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f								//前橋、奥橋
	);

	//ビュー変換行列の計算
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	constMap->view = matView;
	constMap->projection = matProjection;
}

void Camera::InitializeData(Vector3 stageSize)
{
	//ステージの真ん中

	stageMid = { stageSize.x / 2,stageSize.y / 2, stageSize.z / 2 };

	stagePos[0] = { stageSize.x,stageSize.y + (addSizeY * 2),stageSize.z };

	//上
	stagePos[1] = { stageMid.x,stageSize.y + addSizeY,stageSize.z + addSizeZ };
	//下
	stagePos[2] = { stageMid.x,stageSize.y + addSizeY,0 - addSizeZ };
	//左
	stagePos[3] = { 0 - addSizeX ,stageSize.y + addSizeY,	stageMid.z };
	//右
	stagePos[4] = { stageSize.x + addSizeX,stageSize.y + addSizeY, 	stageMid.z };

	isMoveEye = false;

}

void Camera::Update(XMFLOAT3 pPos)
{

	UpdateTarget(pPos);

	UpdateEye();

	UpdateMatrix();

}

void Camera::UpdateEye()
{

	if (isMoveEye == true) {

		MoveEye();

	}
	else {

		if (state_ == 0) {

			eye = target;
			eye.y = stagePos[0].y;
			eye.z = eye.z - 2.5f;

			//placePos[0].x = target.x;
			//placePos[0].y = target.y + addSizeY;
			//placePos[0].z = target.z - 2.5f;

		}
		else if (state_ == 1) {

			eye.x = stagePos[1].x;
			eye.y = stagePos[1].y;
			eye.z = stagePos[1].z;

		}
		else if (state_ == 2) {

			eye.x = stagePos[2].x;
			eye.y = stagePos[2].y;
			eye.z = stagePos[2].z;

		}
		else if (state_ == 3) {

			eye.x = stagePos[3].x;
			eye.y = stagePos[3].y;
			eye.z = stagePos[3].z;

		}
		else if (state_ == 4) {

			eye.x = stagePos[4].x;
			eye.y = stagePos[4].y;
			eye.z = stagePos[4].z;

		}
	}
}

void Camera::UpdateTarget(XMFLOAT3 pPos)
{


	if (state_ == 0) {
		//カメラ視点座標は自機に追従
		target.x = pPos.x;
		target.y = pPos.y;
		target.z = pPos.z;
	}
	else {
		target.x = stageMid.x;
		target.y = 2;
		target.z = stageMid.z;
	}


}

void Camera::ChangeState(int state)
{

	//ベジエ移動実行中をtrue
	isMoveEye = true;
	//移動先をセット
	nextState = state;

	//eye　ベジエリセット

	elapsedCountEye = 0.0f;

	//state == 0 のx.zは常に変わっているためここで情報を更新
	if (state_ == 0 || nextState == 0) {
		stagePos[0].x = target.x;
		stagePos[0].z = target.z - 2.5f;
	}

	startEye = Vector3(stagePos[state_].x, stagePos[state_].y, stagePos[state_].z);	//スタート地点
	endEye = Vector3(stagePos[nextState].x, stagePos[nextState].y, stagePos[nextState].z);	//エンド地点

	//スタート地点とエンド地点の真ん中を制御店に

	float pX;

	if (startEye.x > endEye.x) {
		pX = startEye.x - endEye.x;
	}
	else {
		pX = endEye.x - startEye.x;
	}

	float pZ;

	if (startEye.z > endEye.z) {
		pZ = startEye.z - endEye.z;
	}
	else {
		pZ = endEye.z - startEye.z;
	}

	pEye = Vector3(pX / 2, addSizeY * 2, pZ / 2);	//制御点

	this->state_ = state;

}

void Camera::MoveEye()
{

	//経過時間(elapsedTime [s])の計算
	elapsedCountEye++;
	float elapsedTime = static_cast<float> (elapsedCountEye) / 60.0f;

	//スタート地点			: start
	// 制御点				: p
	//エンド地点			: end
	//経過時間			: elapsedTime [s]
	//移動官僚の率(経過時間/全体時間) : timeRate (%)

	timeRate = elapsedTime / maxTime;

	if (timeRate >= 1) {
		isMoveEye = false;
	}

	Vector3 a = lerp(startEye, pEye, timeRate);
	Vector3 b = lerp(pEye, endEye, timeRate);

	cameraPosition = lerp(a, b, timeRate);

	eye.x = cameraPosition.x;
	eye.y = cameraPosition.y;
	eye.z = cameraPosition.z;

}
