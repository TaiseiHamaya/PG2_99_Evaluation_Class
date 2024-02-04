#include "BaseSceneTransition.h"

/////////////////////////////
/// トランジションの使い方 ///
///////////////////////////
/// ヘッダーは以下のテンプレートを使用する
/*
#include "BaseScene.h"

class トランジション名 : public BaseScene {
public:
	トランジション名() = default;
	~トランジション名() = default;

public:
	void Update() override;
	void DrawBack() const override;
	void DrawFront() const override;
	void Debug() override;
};
*/
/*
IsUpdateInitialScene
IsUpdateTerminalScene
IsDrawInitialScene
IsDrawTerminalScene
IsEndTransition
の5つの関数は、オーバーライドがほぼ必須になるので、必要な戻り値が返る関数にすること。
*/
/// 
/// 

bool BaseSceneTransition::IsUpdateInitialScene() const {
	return true;
}

bool BaseSceneTransition::IsUpdateTerminalScene() const {
	return false;
}

bool BaseSceneTransition::IsDrawInitialScene() const {
	return true;
}

bool BaseSceneTransition::IsDrawTerminalScene() const {
	return false;
}

bool BaseSceneTransition::IsEndTransition() const {
	return true;
}
