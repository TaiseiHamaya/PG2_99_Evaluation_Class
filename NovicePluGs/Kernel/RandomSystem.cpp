#include "RandomSystem.h"

std::unique_ptr<RandomSystem> RandomSystem::instance;

void RandomSystem::Initialize() {
	instance.reset(new RandomSystem{});
}

int RandomSystem::Random_01_Int() {
	return instance->random01_int(instance->engine);
}

float RandomSystem::Random_01_Float() {
	return instance->random01_float(instance->engine);
}

float RandomSystem::Random_11_Float() {
	return instance->random11_float(instance->engine);
}

std::mt19937& RandomSystem::GetEngine() {
	return instance->engine;
}
