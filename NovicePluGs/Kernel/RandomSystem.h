#pragma once

#include <memory>
#include <random>

class RandomSystem {
private:
	RandomSystem() = default;

public:
	RandomSystem(const RandomSystem&) = delete;
	RandomSystem& operator=(const RandomSystem&) = delete;

public:
	static void Initialize();

	/// <summary>
	/// 0または1のint型乱数生成
	/// </summary>
	/// <returns></returns>
	static int Random_01_Int();

	/// <summary>
	/// (0-1]のfloat型乱数生成(1.0fが出る可能性あります)
	/// </summary>
	/// <returns></returns>
	static float Random_01_Float();

	/// <summary>
	/// (-1-1]のfloat型乱数生成(1.0fが出る可能性があります)
	/// </summary>
	/// <returns></returns>
	static float Random_11_Float();

	static std::mt19937& GetEngine();

private:
	static std::unique_ptr<RandomSystem> instance;

private:
	std::random_device seed;
	std::mt19937 engine{ seed() };
	std::uniform_int_distribution<int> random01_int{ 0, 1 };
	std::uniform_real_distribution<float> random01_float{ 0.0f, 1.0f };
	std::uniform_real_distribution<float> random11_float{ -1.0f, 1.0f };
};
