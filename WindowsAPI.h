#pragma once
#include<Windows.h>

class WindowsAPI final
{
private:
	WindowsAPI();
	~WindowsAPI();

public:
	//コピーコンストラクタ無効
	WindowsAPI(const WindowsAPI& obj) = delete;
	//代入演算子を無効
	WindowsAPI& operator=(const WindowsAPI& obj) = delete;

	//インスタンスアクセス専用関数
	static WindowsAPI* GetInstance();

	static const int winW = 1280;
	static const int winH = 720;
	WNDCLASSEX w{};//ウィンドウクラス
	RECT wrc{};//ウィンドウサイズ
	HWND hwnd{};//ウィンドウオブジェクト
	
	
public:

	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void Initialize();
	void Finalize();
	HWND GetHwnd()const { return hwnd; }
	HINSTANCE GetHInstance()const { return w.hInstance; }
	bool ProcessMessage();
};

