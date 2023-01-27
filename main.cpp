#include"MyGame.h"
#include"KEngineFramework.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	KEngineFramework* game = new MyGame();
	
	game->Run();

	delete game;

	return 0;
}
