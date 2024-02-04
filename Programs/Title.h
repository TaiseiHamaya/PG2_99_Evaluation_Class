#pragma once
#include "BaseScene.h"
class Title : public BaseScene {
public:
	Title() = default;
	~Title() = default;

public:
	void Initialize() override;
	void Begin() override;
	void Update() override;
	void BeginRendering() override;
	void Draw() const override;
	void Debug() override;
	void DefaultTransitionUpdate() override;
};

