#include "BoxFall.h"

#include <Novice.h>
#include <RandomSystem.h>
#include <Definition.h>

BoxFall::BoxFall() {
	timer = 0;
	division = 20;
	gravity = { 0.0f,0.8f };
	size = 800 / division;
	boxPos.assign(division, {});
	for (int i = 0; i < boxPos.size(); ++i) {
		boxPos[i].assign(20, Vector2{ float(size * i), -float(size) });
	}
	boxVel.assign(division, std::vector<Vector2>(division, zeroVec2));
	fallBoxIndex.assign(division, 0);
	isCompleted.assign(division, false);
	random = std::make_unique<std::uniform_int_distribution<int>>(5, 20);
	nextFallTime.assign(division, 0);
	for (int i = 0; i < nextFallTime.size(); ++i) {
		nextFallTime[i] = (*random)(RandomSystem::GetEngine());
	}
	completedCount = 0;
}

void BoxFall::Update() {
	++timer;
	if (completedCount < 20) {
		for (int i = 0; i < nextFallTime.size(); ++i) {
			--nextFallTime[i];
			if (nextFallTime[i] <= 0 && fallBoxIndex[i] < division) {
				++fallBoxIndex[i];
				nextFallTime[i] = (*random)(RandomSystem::GetEngine());
			}
		}
		for (int i = 0; i < boxPos.size(); ++i) {
			if (isCompleted[i]) {
				continue;
			}
			for (int j = 0; j < fallBoxIndex[i]; ++j) {
				boxVel[i][j] += gravity;
				boxPos[i][j] += boxVel[i][j];
				if (boxPos[i][j].y >= 800 - size * (j + 1)) {
					if (j == division - 1 && !isCompleted[i]) {
						++completedCount;
						isCompleted[i] = true;
					}
					boxPos[i][j].y = 800.0f - size * (j + 1);
				}
			}
		}
		if (completedCount == 20) {
			for (int i = 0; i < nextFallTime.size(); ++i) {
				nextFallTime[i] = (*random)(RandomSystem::GetEngine());
			}
			boxVel.assign(division, std::vector<Vector2>(division, zeroVec2));
			fallBoxIndex.assign(division, 19);
			isCompleted.assign(division, false);
		}
	}
	else {
		for (int i = 0; i < nextFallTime.size(); ++i) {
			--nextFallTime[i];
			if (nextFallTime[i] <= 0 && fallBoxIndex[i] > 0) {
				--fallBoxIndex[i];
				nextFallTime[i] = (*random)(RandomSystem::GetEngine());
			}
		}
		for (int i = 0; i < boxPos.size(); ++i) {
			if (isCompleted[i]) {
				continue;
			}
			for (int j = division - 1; j >= fallBoxIndex[i]; --j) {
				boxVel[i][j] += gravity;
				boxPos[i][j] += boxVel[i][j];
				if (boxPos[i][j].y >= 800 + size * (j + 1)) {
					if (j == 0 && !isCompleted[i]) {
						++completedCount;
						isCompleted[i] = true;
					}
				}
			}
		}
	}
}

void BoxFall::DrawFront() const {
}

void BoxFall::DrawBack() const {
	for (int i = 0; i < boxPos.size(); ++i) {
		for (int j = 0; j < boxPos[i].size(); ++j) {
			Novice::DrawBox(int(boxPos[i][j].x), int(boxPos[i][j].y), size, size, 0, WHITE, kFillModeSolid);
		}
	}
}

void BoxFall::Debug() {
}

bool BoxFall::IsUpdateInitialScene() const {
	return completedCount < 20;
}

bool BoxFall::IsUpdateTerminalScene() const {
	return completedCount >= 20;
}

bool BoxFall::IsDrawInitialScene() const {
	return completedCount < 20;
}

bool BoxFall::IsDrawTerminalScene() const {
	return completedCount >= 20;
}

bool BoxFall::IsEndTransition() const {
	return completedCount == 40;
}
