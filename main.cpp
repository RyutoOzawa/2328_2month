#include"MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyGame myGame;
	//ゲーム初期化処理
	myGame.Initialize();

	// ゲームループ
	while (true) {

		//ゲーム更新処理
		myGame.Update();
		//終了リクエストがあればループを抜ける
		if (myGame.IsEndReqest()) {
			break;
		}
		//描画処理
		myGame.Draw();
	}

	//終了処理
	myGame.Finalize();

	return 0;
}
