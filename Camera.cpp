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

void Camera::Initialize(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
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
	placePos[0] = { stageSize.x,stageSize.y + (addSizeY  * 2),stageSize.z };

	//ステージのX,Zそれぞれ真ん中
	float midX = stageSize.x / 2;
	float midZ = stageSize.z / 2;

	//上
	placePos[1] = { midX,stageSize.y + addSizeY,stageSize.z + addSizeZ };
	//下
	placePos[2] = { midX,stageSize.y + addSizeY,0 - addSizeZ };
	//左
	placePos[3] = { 0 - addSizeX ,stageSize.y + addSizeY,midZ };
	//右
	placePos[4] = { stageSize.x + addSizeX,stageSize.y + addSizeY, midZ };

	isMove = false;

}

void Camera::UpdateEye()
{



	if (isMove == true) {

		MoveEye();

	}
	else {

		if (state == 0) {

			eye = target;
			eye.y = placePos[0].y;
			eye.z = eye.z - 2.5f;

			//placePos[0].x = target.x;
			//placePos[0].y = target.y + addSizeY;
			//placePos[0].z = target.z - 2.5f;

		}
		else if (state == 1) {

			eye.x = placePos[1].x;
			eye.y = placePos[1].y;
			eye.z = placePos[1].z;

		}
		else if (state == 2) {

			eye.x = placePos[2].x;
			eye.y = placePos[2].y;
			eye.z = placePos[2].z;

		}
		else if (state == 3) {

			eye.x = placePos[3].x;
			eye.y = placePos[3].y;
			eye.z = placePos[3].z;

		}
		else if (state == 4) {

			eye.x = placePos[4].x;
			eye.y = placePos[4].y;
			eye.z = placePos[4].z;

		}
	}
}

void Camera::ChangeEye(int place)
{

	isMove = true;
	nextState = place;


	elapsedCount = 0.0f;

	//state == 0 のx.zは常に変わっているためここで情報を更新
	if (state == 0 || nextState == 0) {
		placePos[0].x = target.x;
		placePos[0].z = target.z - 2.5f;
	}

	start = Vector3(placePos[state].x, placePos[state].y, placePos[state].z);	//スタート地点
	end = Vector3(placePos[nextState].x, placePos[nextState].y, placePos[nextState].z);	//エンド地点

	//スタート地点とエンド地点の真ん中を制御店に

	float pX;

	if (start.x > end.x) {
		pX = start.x - end.x;
	}
	else {
		pX = end.x - start.x;
	}

	float pZ;

	if (start.z > end.z) {
		pZ = start.z - end.z;
	}
	else {
		pZ = end.z - start.z;
	}

	p = Vector3(pX / 2, addSizeY * 2, pZ / 2);	//制御点

	state = place;

}

void Camera::MoveEye()
{

	//経過時間(elapsedTime [s])の計算
	elapsedCount++;
	float elapsedTime = static_cast<float> (elapsedCount) / 60.0f;

	//スタート地点			: start
	// 制御点				: p
	//エンド地点			: end
	//経過時間			: elapsedTime [s]
	//移動官僚の率(経過時間/全体時間) : timeRate (%)

	timeRate = elapsedTime / maxTime;

	if (timeRate >= 1) {
		isMove = false;
	}

	Vector3 a = lerp(start, p, timeRate);
	Vector3 b = lerp(p, end, timeRate);

	cameraPosition = lerp(a, b, timeRate);

	eye.x = cameraPosition.x;
	eye.y = cameraPosition.y;
	eye.z = cameraPosition.z;

}
