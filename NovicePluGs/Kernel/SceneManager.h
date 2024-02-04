#pragma once
class SceneChange;
class BaseScene;
class BaseSceneTransition;

#include <memory>
#include <deque>
#include <functional>

/// <summary>
/// シーン管理用クラス
/// </summary>
class SceneManager final {
	friend class SceneChange;
public:
	SceneManager() = delete;
	~SceneManager() = delete;

public:
	static void Initialize(BaseScene* ptr);
	static void Finalize();

	static void Update();
	static void Draw();
	static void Debug();
	static void SetSceneChange(
		BaseScene* nextScenePtr, BaseSceneTransition* sceneTransition,
		bool isStackInitialScene = false,
		std::function<void(void)> initialSceneUpdate = nullptr,
		std::function<void(void)> terminalSceneUpdat = nullptr
	);
	static void PopScene(
		BaseSceneTransition* sceneTransition,
		std::function<void(void)> initialSceneUpdate = nullptr,
		std::function<void(void)> terminalSceneUpdat = nullptr
	);
	static bool IsEndProgram();

private:
	static void SetNextScene();

private:
	/// <summary>
	/// 現在のシーン
	/// </summary>
	static std::deque<std::unique_ptr<BaseScene>> sceneQue;
};
