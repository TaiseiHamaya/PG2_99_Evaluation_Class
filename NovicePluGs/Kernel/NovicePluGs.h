#pragma once

class NovicePluGs final {
public:
	//コピー禁止
	NovicePluGs(const NovicePluGs&) = delete;
	NovicePluGs& operator=(const NovicePluGs&) = delete;

	/// <summary>
	/// NovicePluGs及びNoviceの初期化
	/// <param name="kWindowTitle">ウィンドウタイトル</param>
	/// <param name="windowSizeX">ウィンドウのX方向の大きさ</param>
	/// <param name="windowSizeY">ウィンドウのY方向の大きさ</param>
	/// </summary>
	static void
		Initialize(const char* kWindowTitle, const float windowSizeX, const float windowSizeY);

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	static void Run();
};