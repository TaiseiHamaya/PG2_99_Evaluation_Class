#pragma once

#include "BaseSceneTransition.h"

#include <vector>
#include <Vector2d.h>
#include <random>
#include <memory>

class BoxFall : public BaseSceneTransition {
public:
	BoxFall();
	~BoxFall() = default;

public:
	void Update() override;
	void DrawFront() const override;
	void DrawBack() const override;
	void Debug() override;

	bool IsUpdateInitialScene() const;
	bool IsUpdateTerminalScene() const;
	bool IsDrawInitialScene() const;
	bool IsDrawTerminalScene() const;
	bool IsEndTransition() const;

private:
	int division;
	int timer;
	int size;
	int completedCount;
	Vector2 gravity;
	std::vector<std::vector<Vector2>> boxPos;
	std::vector<std::vector<Vector2>> boxVel;
	std::vector<int> fallBoxIndex;
	std::vector<int> isCompleted;
	std::vector<int> nextFallTime;
	std::unique_ptr<std::uniform_int_distribution<int>> random;
};

