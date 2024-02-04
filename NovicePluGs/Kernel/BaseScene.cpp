#include "BaseScene.h"

/////////////////////
/// シーンの使い方 ///
////////////////////
/// ヘッダーは以下のテンプレートを使用する
/*
#include "BaseScene.h"

class シーン名 : public BaseScene {
public:
	シーン名() = default;
	~シーン名() = default;

public:
	void Begin() override;
	void Update() override;
	void BeginRendering() override;
	void Draw() const override;
	void Debug() override;
	void DefaultTransitionUpdate() override;
};
*/
///
/// もし、シーン遷移が発生するなら
/// 行き先のシーン名* temp = new 行き先のシーン名();
/// SceneManager::SetSceneChange(temp, new 使用するシーントランジションクラス名(), true or false, std::bind(&元のクラス::実行したいUpdate関数名, this), std::bind(&次ののクラス::実行したいUpdate関数名, temp));
/// の2つを呼び出す。(SceneMnagerクラス、SceneChangeクラス、行き先クラスのヘッダーのインクルード忘れに注意する。)
/// また、現在のシーンを破棄するかどうかのtrue,falseと、std::bind2つは省略可能であり、それぞれ自動的に「false(破棄する)」、「NonInputUpdateに記述した内容を使用する」となる。
/// new SceneChange(引数);の引数の詳細は、SceneChange.hに書いてあるので、それを参照すること。
///
/// Update関数内で更新処理、Draw関数内で描画処理を行うこと！！！

void BaseScene::Initialize() {
}

void BaseScene::Begin() {
}

void BaseScene::Update() {
}

void BaseScene::BeginRendering() {
}

void BaseScene::Draw() const {
}

void BaseScene::Debug() {
}

void BaseScene::DefaultTransitionUpdate() {
}
