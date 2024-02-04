#pragma once

class BaseSceneTransition {
public:
	BaseSceneTransition() = default;
	virtual ~BaseSceneTransition() = default;

	virtual void Update() = 0;
	virtual void DrawBack() const = 0;
	virtual void DrawFront() const = 0;
	virtual void Debug() = 0;

	virtual bool IsUpdateInitialScene() const;
	virtual bool IsUpdateTerminalScene() const;
	virtual bool IsDrawInitialScene() const;
	virtual bool IsDrawTerminalScene() const;
	virtual bool IsEndTransition() const;
};