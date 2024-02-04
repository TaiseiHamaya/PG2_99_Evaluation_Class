#pragma once

#include "BaseScene.h"

class Result : public BaseScene {
public:
	Result(int result);
	~Result() = default;

public:
	void Initialize() override;
	void Begin() override;
	void Update() override;
	void BeginRendering() override;
	void Draw() const override;
	void Debug() override;
	void DefaultTransitionUpdate() override;

private:
	int killCount;
};

