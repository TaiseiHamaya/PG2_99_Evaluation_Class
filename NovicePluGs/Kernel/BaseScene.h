#pragma once

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene {
public:
	BaseScene() = default;
	virtual ~BaseScene() = default;

private: /// --------------コピー禁止--------------
	BaseScene(const BaseScene&) = delete;
	BaseScene& operator=(const BaseScene&) = delete;

public:
	virtual void Initialize();
	virtual void Begin();
	virtual void Update();
	virtual void BeginRendering();
	virtual void Draw() const;
	virtual void Debug();
	virtual void DefaultTransitionUpdate();
};