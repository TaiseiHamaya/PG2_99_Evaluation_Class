#pragma once

#include <BaseScene.h>

class Game final : public BaseScene {
public:
	Game();
	~Game();

	static void Load();

public:
	void Initialize() override;
	void Begin() override;
	void Update() override;
	void BeginRendering() override;
	void Draw() const override;
	void Debug() override;
	void DefaultTransitionUpdate() override;

private:
	bool isEnd;
	int endTimer;
};