#pragma once

#include "BaseSceneTransition.h"

class Moment : public BaseSceneTransition {
public:
	Moment();
	~Moment() = default;

public:
	void Update() override;
	void DrawFront() const override;
	void DrawBack() const override;
	void Debug() override;

private:
};