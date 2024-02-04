#include "SceneManager.h"

#include "BaseScene.h"
#include "SceneChange.h"
#include "BaseSceneTransition.h"
#include "Definition.h"

#include <Novice.h>
#include <assert.h>

std::deque<std::unique_ptr<BaseScene>> SceneManager::sceneQue;

void SceneManager::Initialize(BaseScene* ptr) {
	assert(sceneQue.empty());
	sceneQue.push_back(std::unique_ptr<BaseScene>(nullptr));
	sceneQue.push_back(std::unique_ptr<BaseScene>(ptr));
	ptr->Initialize();
}

void SceneManager::Finalize() {
	sceneQue.clear();
}


void SceneManager::Update() {
	sceneQue.back()->Begin();
	sceneQue.back()->Update();
	sceneQue.back()->BeginRendering();
}

void SceneManager::Draw() {
	sceneQue.back()->Draw();
}

void SceneManager::Debug() {
	Novice::ScreenPrintf(0, 0, "%d", sceneQue.size());
	Novice::ScreenPrintf(0, 20, "%p", sceneQue.back().get());
	sceneQue.back()->Debug();
}

void SceneManager::SetSceneChange(BaseScene* nextScenePtr, BaseSceneTransition* sceneTransition,
	bool isStackInitialScene, std::function<void(void)> initialSceneUpdate, std::function<void(void)> terminalSceneUpdat) {
	std::unique_ptr<SceneChange> temp(new SceneChange(sceneQue.back().release(), nextScenePtr, sceneTransition, isStackInitialScene, initialSceneUpdate, terminalSceneUpdat));
	nextScenePtr->Initialize();
	sceneQue.back() = std::move(temp);
}

void SceneManager::SetNextScene() {
	SceneChange* temp = dynamic_cast<SceneChange*>(sceneQue.back().release());
	sceneQue.pop_back();
	if (temp->GetIsStackInitialScene()) {
		sceneQue.emplace_back(std::move(temp->GetInitialScene()));
	}
	sceneQue.emplace_back(std::move(temp->GetTerminalScene()));
	MemClear(temp);
}

void SceneManager::PopScene(BaseSceneTransition* sceneTransition,
	std::function<void(void)> initialSceneUpdate, std::function<void(void)> terminalSceneUpdat) {
	assert(sceneQue.size() >= 2);	
	std::unique_ptr<SceneChange> temp(new SceneChange(sceneQue.back().release(), (*(sceneQue.end() - 2)).release(), sceneTransition, false, initialSceneUpdate, terminalSceneUpdat));
	sceneQue.pop_back();
	sceneQue.back() = std::move(temp);
}

bool SceneManager::IsEndProgram() {
	return sceneQue.back() == nullptr;
}
