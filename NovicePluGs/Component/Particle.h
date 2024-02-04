#pragma once
#include <Novice.h>

#include "Vector2D.h"
#include "Color.h"

#include <random>
#include <vector>

struct ParticleProperties;

enum ParticleDrawType {
	kParticleDrawTypeNull,   // NULL(Default)
	kParticleDrawTypeSphere, // 円
	kParticleDrawTypeBox,    // 四角
	kParticleDrawTypeLine,   // 直線
	kParticleDrawTypeSprite  // スプライト描画
};

enum ParticleSpawnType {
	kParticleSpawnTypeNull,   // NULL(Default)
	kParticleSpawnTypeSphere, // 円
	kParticleSpawnTypeBox,    // 四角
	kParticleSpawnTypeLine,   // 直線
	kParticleSpawnTypePoint   // 点
};

/// <summary>
/// パーティクル
/// </summary>
class Particle {
public:
	/// <summary>
	/// 初期化用コンストラクタ (※エラー防止にParticleAssertを必ず実行する!)
	/// </summary>
	/// <param name="drawType">描画タイプ設定</param>
	/// <param name="spawnType">発生タイプ設定</param>
	/// <param name="duration">実行するフレーム数</param>
	/// <param name="rate">1回に発生する数</param>
	/// <param name="particlesParTime">1回発生してから次に発生するまでのフレーム数</param>
	Particle(
		ParticleDrawType drawType = kParticleDrawTypeNull, ParticleSpawnType spawnType = kParticleSpawnTypeNull,
		int duration = 0, int rate = 0, int particlesParTime = 0
	);

	~Particle() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() const ;

	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();

	/// <summary>
	/// 生成
	/// </summary>
	void Generate();

public:
	/// <summary>
	/// 描画に必要なプロパティ
	/// </summary>
	/// <param name="setFillMode">塗りつぶしタイプ</param>
	void SetDrawTypeProperties(FillMode setFillMode);

	/// <summary>
	/// 描画に必要なプロパティ
	/// </summary>
	/// <param name="setSpriteHandle">スプライト名</param>
	void SetDrawTypeProperties(std::string spriteNum);

	/// <summary>
	/// スポーンに必要なプロパティ
	/// </summary>
	/// <param name="setSpwanRange">範囲</param>
	void SetSpawnTypeProperties(const Vector2& setSpwanRange);

	/// <summary>
	/// ループ処理するか
	/// </summary>
	/// <param name="setLoop">[true, false]</param>
	void SetIsLoop(bool setLoop);

	/// <summary>
	/// エミッターを設定
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="velocity">速度</param>
	/// <param name="acceleraration">加速度</param>
	/// <param name="size">大きさ</param>
	/// <param name="startRotate">初期回転</param>
	/// <param name="rotateSpeed">回転速度</param>
	/// <param name="color">色</param>
	/// <param name="lifeTime">寿命</param>
	void SetEmitter(
		const Vector2& position, const Vector2& velocity, const Vector2& acceleraration, const Vector2& size,
		float startRotate, float rotateSpeed, int lifeTime, unsigned int color, unsigned int targetColor = 0xffffffff + 1
	);

	/// <summary>
	/// パーティクルのランダム化用プロパティ
	/// </summary>
	/// <param name="setRandomVelocityRange">速度の変化範囲 [0.0f, PI]</param>
	/// <param name="setRandomVelocityMagnification">速度の大きさをランダム化 [0.0f, 1.0f]</param>
	/// <param name="setRandomAccelerarationRange">加速度の変化範囲 [0.0f, PI]</param>
	/// <param name="setRandomAccelerarationMagnification">加速度の大きさのランダム化 [0.0f, 1.0f]</param>
	/// <param name="setIsRandomizeRotationDirection">回転方向をランダム化するかどうか bool</param>
	/// <param name="setIsRandomizeStartRotation">初期回転量をランダム化するかどうか bool</param>
	/// <param name="setRandomRotateSpeed">回転速度のランダム化 [0.0f, 1.0f]</param>
	/// <param name="setRandomSize">大きさのランダム化 [0.0f, 1.0f]</param>
	/// <param name="setRandomLifeTime">寿命のランダム化 [0.0f, 1.0f]</param>
	void SetRandomProperties(
		float setRandomVelocityRange, float setRandomVelocityMagnification,
		float setRandomAccelerarationRange, float setRandomAccelerarationMagnification,
		bool setIsRandomizeRotationDirection, bool setIsRandomizeStartRotation,
		float setRandomRotateSpeed, float setRandomSize, float setRandomLifeTime
	);

	/// <summary>
	/// エラー確認用※必ず実行する!!!
	/// </summary>
	void ParticleAssert();

	void SetEmitterPos(const Vector2& position);

	void SetEmittorVelocity(const Vector2& velocity_);

	void SetRandomVelocityRange(float range);

	#ifdef _DEBUG
	void DebugGUI();
	#endif // _DEBUG

private:
	// パーティクルの基本構造体
	struct ParticleProperties {
		int currentTime;            // タイマー
		Vector2 position;          // パーティクルの位置
		Vector2 velocity;     // 速度
		Vector2 acceleration; // 加速度
		Vector2 size;         // 大きさ

		float startRotate;  // 初期回転量
		float rotateSpeed;  // 回転速度
		Color color; // 色
		int lifeTime;       // 寿命

		Color targetColor; // 色の変化S

	public:
		ParticleProperties() = default;
		ParticleProperties(const ParticleProperties& emitter, Particle& randomize, const  Vector2& spawnPos);

		void Update();
		bool IsErase();
	};

private:
	int currentTime; // タイマー
	// 必須プロパティ
	ParticleDrawType drawType;  // 描画タイプ
	ParticleSpawnType spawnType; // スポーンタイプ
	int duration;            // 継続時間
	int rate;                // 1回に発生するパーティクル数
	int particlesParTime;    // パーティクルが発生するフレーム間隔

	// 準必須プロパティ
	FillMode fillMode;  // 描画時の塗りつぶし(pdSpriteの場合は無視されます)
	Vector2 spawnRange; // スポーン時の範囲(psPointの場合は無視されます)
	std::string spriteName;

	/// 任意プロパティ
	bool isLoop; // ループするかどうか
	// 動きをランダム化するプロパティ
	std::random_device seed;     // 非決定的な乱数生成器を生成
	std::mt19937 engine{ seed() }; //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<int> random01_int{ 0, 1 };
	std::uniform_real_distribution<float> random01_float{ 0.0f, 1.0f };
	std::uniform_real_distribution<float> random11_float{ -1.0f, 1.0f };

	float randomVelocityRange;               // ランダム化した速度の向きの範囲(ラジアン)
	float randomVelocityMagnification;       // ランダム化した速度の大きさ(1を基準とした倍率)
	float randomAccelerarationRange;         // ランダム化した加速度の向きの範囲(ラジアン)
	float randomAccelerarationMagnification; // ランダム化した速度の大きさ(1を基準とした倍率)
	bool isRandomizeRotationDirection;       // 回転する向きをランダムにするか
	bool isRandomizeStartRotation;           // 初期状態の回転量をランダム化するか
	float randomRotateSpeed;                 // 回転速度のランダム化
	float randomSize;                        // 大きさを+-この値分だけ割合としてランダムに増減
	float randomLifeTime;                    // 生存時間を+-この値分だけ割合としてランダムに増減

	// エミッター
	ParticleProperties emitter;

	// 生成後のパーティクル
	std::vector<ParticleProperties> particles;

	bool isChangeFillMode;
};