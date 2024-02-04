#pragma once

#include "BaseScene.h"

#include <functional>
#include <memory>

#include "BaseSceneTransition.h"

class SceneChange final : public BaseScene {
	friend class SceneManager;//コンストラクタアクセス用
public:
	~SceneChange() = default;

private: /// --------------コピー禁止--------------
	SceneChange(const SceneChange&) = delete;
	SceneChange& operator=(const SceneChange&) = delete;

private://SceneManger以外で生成させない
	SceneChange() = default;
	explicit SceneChange(
		BaseScene* initialScene_, BaseScene* terminalScene_, BaseSceneTransition* sceneTransition,
		bool isStackInitialScene = false,
		std::function<void(void)> initialSceneUpdate = nullptr,
		std::function<void(void)> terminalSceneUpdat = nullptr
	);

public:
	void Update() override;
	void Draw() const override;
	void Debug() override;
	void DefaultTransitionUpdate() override;

public:
	bool IsEndSceneChange() const;
	std::unique_ptr<BaseScene>& GetInitialScene();
	std::unique_ptr<BaseScene>& GetTerminalScene();
	const std::unique_ptr<BaseScene>& GetInitialScene() const;
	const std::unique_ptr<BaseScene>& GetTerminalScene() const;
	const bool GetIsStackInitialScene() const;

private:
	std::unique_ptr<BaseScene> initialScene;
	std::unique_ptr<BaseScene> terminalScene;
	const std::unique_ptr<BaseSceneTransition> sceneTransition;
	const std::function<void(void)> initialSceneUpdate;
	const std::function<void(void)> terminalSceneUpdate;
	const bool isStackInitialScene;

	int timer;
};