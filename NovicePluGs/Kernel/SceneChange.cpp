#include "SceneChange.h"

#include "SceneManager.h"

#include <Novice.h>
#include <cassert>

SceneChange::SceneChange(
	BaseScene* initialScene_, BaseScene* terminalScene_, BaseSceneTransition* sceneTransition, bool isStackInitialScene,
	std::function<void(void)> initialSceneUpdate, std::function<void(void)> terminalSceneUpdate
) :
	sceneTransition(std::move(sceneTransition)),
	isStackInitialScene(isStackInitialScene),
	initialSceneUpdate(initialSceneUpdate),
	terminalSceneUpdate(terminalSceneUpdate) {
	assert(!initialScene);
	assert(!terminalScene);
	this->initialScene.reset(initialScene_);
	this->terminalScene.reset(terminalScene_);

	timer = 0;
}

void SceneChange::Update() {
	timer++;
	if (sceneTransition->IsUpdateInitialScene()) {
		if (initialSceneUpdate) {
			initialSceneUpdate();
		}
		else {
			initialScene->DefaultTransitionUpdate();
		}
	}
	if (sceneTransition->IsUpdateTerminalScene()) {
		if (terminalSceneUpdate) {
			terminalSceneUpdate();
		}
		else {
			terminalScene->DefaultTransitionUpdate();
		}
	}

	sceneTransition->Update();
	if (IsEndSceneChange()) {
		SceneManager::SetNextScene();
	}
	return;
}

void SceneChange::Draw() const {
	sceneTransition->DrawBack();
	if (sceneTransition->IsDrawInitialScene()) {
		assert(initialScene);
		initialScene->Draw();
	}
	if (sceneTransition->IsDrawTerminalScene()) {
		assert(terminalScene);
		terminalScene->Draw();
	}
	sceneTransition->DrawFront();
}

void SceneChange::Debug() {
	Novice::ScreenPrintf(0, 20, "SceneChange");
	Novice::ScreenPrintf(0, 40, "%d", timer);
	Novice::ScreenPrintf(0, 60, "from %p", initialScene.get());
	Novice::ScreenPrintf(0, 80, "terminal %p", terminalScene.get());
	sceneTransition->Debug();
}

void SceneChange::DefaultTransitionUpdate() {
}

bool SceneChange::IsEndSceneChange() const {
	return sceneTransition->IsEndTransition();
}

std::unique_ptr<BaseScene>& SceneChange::GetInitialScene() {
	return initialScene;
}

std::unique_ptr<BaseScene>& SceneChange::GetTerminalScene() {
	return terminalScene;
}

const std::unique_ptr<BaseScene>& SceneChange::GetInitialScene() const {
	return initialScene;
}

const std::unique_ptr<BaseScene>& SceneChange::GetTerminalScene() const {
	return terminalScene;
}

const bool SceneChange::GetIsStackInitialScene() const {
	return isStackInitialScene;
}
