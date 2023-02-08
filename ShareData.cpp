#include "ShareData.h"
#include"WindowsAPI.h"
using namespace DirectX;

int ShareData::stageNumber = 0;
int ShareData::titlePhase = TitlePhaseIndex::WaitInputSpaceKey;

EaseingData ShareData::sceneChangeEase{  };
XMFLOAT2 ShareData::easePos[2][2] = {
	{{ WindowsAPI::winW / 2.0f,0.0f },{0.0f,0}},
	{{ WindowsAPI::winW / 2.0f,0.0f },{WindowsAPI::winW,0}}
};
XMFLOAT2 ShareData::nextPos[2]{};
bool ShareData::isActiveSceneChange = false;
bool ShareData::isBeforeSceneClosed = false;

void ShareData::OpenSceneChange()
{
	easePos[0][1] = { WindowsAPI::winW / 2.0f,0.0f };
	easePos[0][0] = { 0.0f,0 };
	easePos[1][1] = { WindowsAPI::winW / 2.0f,0.0f };
	easePos[1][0] = { WindowsAPI::winW,0 };

	isActiveSceneChange = true;
	isBeforeSceneClosed = false;
	sceneChangeEase.Start(1.0f);
	nextPos[0] = easePos[0][0];
	nextPos[1] = easePos[1][0];
}

void ShareData::CloseSceneChange()
{
	easePos[0][0] = { WindowsAPI::winW / 2.0f,0.0f };
	easePos[0][1] = { 0.0f,0 };
	easePos[1][0] = { WindowsAPI::winW / 2.0f,0.0f };
	easePos[1][1] = { WindowsAPI::winW,0 };

	isActiveSceneChange = true;
	isBeforeSceneClosed = true;
	sceneChangeEase.Start(1.0f);
	nextPos[0] = easePos[0][0];
	nextPos[1] = easePos[1][0];
}
