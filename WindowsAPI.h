#pragma once
#include<Windows.h>

class WindowsAPI final
{
private:
	WindowsAPI();
	~WindowsAPI();

public:
	//�R�s�[�R���X�g���N�^����
	WindowsAPI(const WindowsAPI& obj) = delete;
	//������Z�q�𖳌�
	WindowsAPI& operator=(const WindowsAPI& obj) = delete;

	//�C���X�^���X�A�N�Z�X��p�֐�
	static WindowsAPI* GetInstance();

	static const int winW = 1280;
	static const int winH = 720;
	WNDCLASSEX w{};//�E�B���h�E�N���X
	RECT wrc{};//�E�B���h�E�T�C�Y
	HWND hwnd{};//�E�B���h�E�I�u�W�F�N�g
	
	
public:

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Initialize();
	void Finalize();
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance()const { return w.hInstance; }
	bool ProcessMessage();
};

