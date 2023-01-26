#include "Input.h"
#include<wrl.h>
#include<cassert>
using namespace Microsoft::WRL;
#include"ImguiManager.h"

void Input::Initialize(WindowsAPI* windowsApi)
{
	HRESULT result_;
	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	this->windowsApi = windowsApi;

	//DirectInput�̃C���X�^���X����
	result_ = DirectInput8Create(
		windowsApi->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result_));
	//�L�[�{�[�h�f�o�C�X����
	result_ = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result_));
	//���̓f�[�^�`���̃Z�b�g
	result_ = keyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	assert(SUCCEEDED(result_));
	//�r�����䃌�x���̃Z�b�g
	result_ = keyboard->SetCooperativeLevel(
		windowsApi->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result_));
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();
	//�O�t���[���̃L�[����ۑ�
	for (int i = 0; i < 256; i++) {
		oldkey[i] = key[i];
	}
	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);

	//�p�b�h�̐ڑ��m�F
	oldPadState = padState;
	DWORD flag = XInputGetState(0, &padState);


	ImGui::Begin("padInput");

	ImGui::Text("leftTrigger : %d", padState.Gamepad.bLeftTrigger);
	ImGui::Text("rightTrigger : %d", padState.Gamepad.bRightTrigger);
	ImGui::Text("lStickX : %d", padState.Gamepad.sThumbLX);
	ImGui::Text("lstickY : %d", padState.Gamepad.sThumbLY);
	ImGui::Text("rStickX : %d", padState.Gamepad.sThumbRX);
	ImGui::Text("rStickY : %d", padState.Gamepad.sThumbRY);
	if (IsDownRStickLeft()) ImGui::Text("stick down left!");
	if (IsDownRStickRight()) ImGui::Text("stick down right!");
	if (IsDownRStickUp()) ImGui::Text("stick down up!");
	if (IsDownRStickDown()) ImGui::Text("stick down down!");

	ImGui::End();


}

bool Input::IsKeyTrigger(BYTE key_)
{
	return (key[key_] && !oldkey[key_]);
}

bool Input::IsKeyPress(BYTE key_)
{
	return key[key_];
}

bool Input::IsKeyRelease(BYTE key_)
{
	return (!key[key_] && oldkey[key_]);
}

bool Input::IsPadTrigger(WORD Button)
{
	return (padState.Gamepad.wButtons & Button) && ((oldPadState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsPadPress(WORD Button)
{
	return padState.Gamepad.wButtons & Button;
}

bool Input::IsPadRelease(WORD Button)
{
	return (oldPadState.Gamepad.wButtons & Button) && ((padState.Gamepad.wButtons & Button) != Button);
}

bool Input::IsDownLStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbLX < -deadZone;
}

bool Input::IsDownLStickRight(int deadZone)
{
	return padState.Gamepad.sThumbLX > deadZone;
}

bool Input::IsDownLStickUp(int deadZone)
{
	return padState.Gamepad.sThumbLY > deadZone;
}

bool Input::IsDownLStickDown(int deadZone)
{
	return padState.Gamepad.sThumbLY < -deadZone;
}

bool Input::IsDownRStickLeft(int deadZone)
{
	return padState.Gamepad.sThumbRX < -deadZone;
}

bool Input::IsDownRStickRight(int deadZone)
{
	return padState.Gamepad.sThumbRX >deadZone;
}

bool Input::IsDownRStickUp(int deadZone)
{
	return padState.Gamepad.sThumbRY >deadZone;
}

bool Input::IsDownRStickDown(int deadZone)
{
	return padState.Gamepad.sThumbRY < -deadZone;
}
