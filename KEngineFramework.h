#pragma once
class KEngineFramework
{
	//KEngineFramework();

	virtual ~KEngineFramework() = default;

	//初期化
	virtual void Initialize();
	
	//終了
	virtual void Finalize();

	//毎フレーム更新
	virtual void Update();

	//描画
	virtual void Draw() = 0;

	//終了チェック
	virtual bool IsEndRequest() { return endRequest; }

	//処理の全実行
	void Run();

	bool endRequest = false;
};

