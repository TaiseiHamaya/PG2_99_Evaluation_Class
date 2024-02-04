#include "DeviceData.h"

std::unique_ptr<DeviceData> DeviceData::instance;

DeviceData::DeviceData(float windowWidth, float windowHight) {
	windowSize = { windowWidth, windowHight };
	screenSize = {};
}

void DeviceData::Initialize(int windowWidth, int windowHight) {
	instance.reset(new DeviceData(float(windowWidth), float(windowHight)));
}

Vector2 DeviceData::GetWindowSize() {
	return instance->windowSize;
}

Vector2 DeviceData::GetScreenSize() {
	return instance->screenSize;
}