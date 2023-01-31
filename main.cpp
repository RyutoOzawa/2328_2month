#include"MyGame.h"
#include"KEngineFramework.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//フレームワークを基底クラスとしたゲームクラスを生成
	KEngineFramework* game = new MyGame();
	
	//ゲームクラスの処理実行
	game->Run();

	//ゲームクラス開放
	delete game;

	return 0;
}
