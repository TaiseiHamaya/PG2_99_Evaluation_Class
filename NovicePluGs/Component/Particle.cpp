#include "Particle.h"

#include <cassert>

#ifdef _DEBUG
#include "imgui.h"
#include <format>
#endif // _DEBUG

#include "Definition.h"
#include "GameTextureManager.h"

Particle::Particle(ParticleDrawType drawType, ParticleSpawnType spawnType, int duration, int rate, int particlesParTime) {
	this->drawType = drawType;
	this->spawnType = spawnType;
	this->duration = duration;
	this->rate = rate;
	this->particlesParTime = particlesParTime;

	fillMode = kFillModeWireFrame;
	spawnRange = { 0, 0 };
	spriteName = "";
	isLoop = false;
	randomVelocityRange = 0;
	randomVelocityMagnification = 0;
	randomAccelerarationRange = 0;
	randomAccelerarationMagnification = 0;
	isRandomizeRotationDirection = false;
	isRandomizeStartRotation = false;
	randomRotateSpeed = 0;
	randomSize = 0;
	randomLifeTime = 0;
	emitter = {};
	particles.clear();

	isChangeFillMode = false;
	currentTime = 0;
}

void Particle::Update() {
	currentTime++;
	if (isLoop && currentTime == duration) {
		currentTime = 0;
	}

	// パーティクル更新
	auto itr = particles.begin();
	while (itr != particles.end()) {
		(*itr).Update();
		if ((*itr).IsErase()) {
			itr = particles.erase(itr);
		}
		else {
			++itr;
		}
	}

	if (!isLoop && duration < currentTime) {
		return;
	}
	// 生成
	if (particlesParTime != 0) {
		if (currentTime % particlesParTime == 0) {
			Generate();
		}
	}
}

void Particle::Draw() const {
	switch (drawType) {
		case kParticleDrawTypeNull:
			assert(!(drawType == kParticleDrawTypeNull)); // エラー出力
			break;

		case kParticleDrawTypeSphere:
			for (int i = 0; i < particles.size(); i++) {
				Novice::DrawEllipse(
					static_cast<int>(particles[i].position.x), static_cast<int>(particles[i].position.y),
					static_cast<int>(particles[i].size.x), static_cast<int>(particles[i].size.y),
					particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime, Color::LerpC(particles[i].color, particles[i].targetColor, float(particles[i].currentTime) / particles[i].lifeTime).hex(), fillMode
				);
			}
			break;

		case kParticleDrawTypeBox:
			for (int i = 0; i < particles.size(); i++) {
				Novice::DrawBox(
					static_cast<int>((particles[i].position + Vector2::Rotate(-particles[i].size, particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position + Vector2::Rotate(-particles[i].size, particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					static_cast<int>(particles[i].size.x * 2), static_cast<int>(particles[i].size.y * 2),
					particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime, Color::LerpC(particles[i].color, particles[i].targetColor, float(particles[i].currentTime) / particles[i].lifeTime).hex(), fillMode
				);
			}
			break;

		case kParticleDrawTypeLine:
			for (int i = 0; i < particles.size(); i++) {
				Novice::DrawLine(
					static_cast<int>((particles[i].position + Vector2::Rotate(-particles[i].size, particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position + Vector2::Rotate(-particles[i].size, particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					static_cast<int>((particles[i].position - Vector2::Rotate(-particles[i].size, particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position - Vector2::Rotate(-particles[i].size, particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					Color::LerpC(particles[i].color, particles[i].targetColor, float(particles[i].currentTime) / particles[i].lifeTime).hex()
				);
			}
			break;

		case kParticleDrawTypeSprite:
			for (int i = 0; i < particles.size(); i++) {
				Novice::DrawQuad(
					static_cast<int>((particles[i].position - Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position - Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					static_cast<int>((particles[i].position + Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position - Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					static_cast<int>((particles[i].position - Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position + Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					static_cast<int>((particles[i].position + Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).x),
					static_cast<int>((particles[i].position + Vector2::Rotate(Vector2::Multiply(particles[i].size, 0.5f), particles[i].startRotate + particles[i].rotateSpeed * particles[i].currentTime)).y),
					0, 0, int(particles[i].size.x), int(particles[i].size.y), GameTextureManager::GetHandle(spriteName), Color::LerpC(particles[i].color, particles[i].targetColor, float(particles[i].currentTime) / particles[i].lifeTime).hex()
				);
			}
			break;

		default:
			throw("Particle DrawType is abnormality."); // エラー出力
			break;
	}
}

void Particle::Debug() {
	Novice::ScreenPrintf(200, 0, "%d %d", drawType, spawnType);
	Novice::ScreenPrintf(200, 20, "%d", currentTime);
	Novice::ScreenPrintf(200, 40, "%d", particles.size());
}

void Particle::Generate() {
	for (int i = 0; i < rate; i++) {
		switch (spawnType) {
			case kParticleSpawnTypeNull:
				assert(!(spawnType == kParticleSpawnTypeNull)); // エラー出力
				break;
			case kParticleSpawnTypeSphere:
				particles.emplace_back(emitter, *this, Vector2::Rotate(Vector2::Multiply(spawnRange, random01_float(engine)), random01_float(engine) * PI * 2));
				break;
			case kParticleSpawnTypeBox:
				particles.emplace_back(emitter, *this, Vector2::Multiply(spawnRange, { random11_float(engine), random11_float(engine) }));
				break;
			case kParticleSpawnTypeLine:
				particles.emplace_back(emitter, *this, Vector2::Multiply(spawnRange, random11_float(engine)));
				break;
			case kParticleSpawnTypePoint:
				particles.emplace_back(emitter, *this, Vector2{ 0, 0 });
				break;
			default:
				throw("SpawnType is abnormality."); // エラー出力
				break;
		}
	}
}

void Particle::SetDrawTypeProperties(FillMode setFillMode) {
	assert(drawType == kParticleDrawTypeBox || drawType == kParticleDrawTypeSphere);
	fillMode = setFillMode;
	isChangeFillMode = true;
}

void Particle::SetDrawTypeProperties(std::string spriteName_) {
	assert(drawType == kParticleDrawTypeSprite);
	spriteName = spriteName_;
}

void Particle::SetSpawnTypeProperties(const Vector2& setSpwanRange) {
	assert(spawnType == kParticleSpawnTypeSphere || spawnType == kParticleSpawnTypeBox || spawnType == kParticleSpawnTypeLine);
	spawnRange = setSpwanRange;
}

void Particle::SetIsLoop(bool setLoop) {
	isLoop = setLoop;
}

void Particle::SetEmitter(const Vector2& position, const Vector2& velocity, const Vector2& acceleraration, const Vector2& size, float startRotate, float rotateSpeed, int lifeTime, unsigned int color, unsigned int targetColor) {
	emitter.position = position;
	emitter.velocity = velocity;
	emitter.acceleration = acceleraration;
	emitter.size = size;
	emitter.startRotate = startRotate;
	emitter.rotateSpeed = rotateSpeed;
	emitter.color = color;
	emitter.lifeTime = lifeTime;
	emitter.currentTime = 0;
	if (targetColor == 0xffffffff + 1) {
		emitter.targetColor = color;
	}
	else {
		emitter.targetColor = targetColor;
	}
}

void Particle::SetRandomProperties(
	float setRandomVelocityRange, float setRandomVelocityMagnification,
	float setRandomAccelerarationRange, float setRandomAccelerarationMagnification,
	bool setIsRandomizeRotationDirection, bool setIsRandomizeStartRotation,
	float setRandomRotateSpeed, float setRandomSize, float setRandomLifeTime
) {
	randomVelocityRange = setRandomVelocityRange;
	randomVelocityMagnification = setRandomVelocityMagnification;
	randomAccelerarationRange = setRandomAccelerarationRange;
	randomAccelerarationMagnification = setRandomAccelerarationMagnification;
	isRandomizeRotationDirection = setIsRandomizeRotationDirection;
	isRandomizeStartRotation = setIsRandomizeStartRotation;
	randomRotateSpeed = setRandomRotateSpeed;
	randomSize = setRandomSize;
	randomLifeTime = setRandomLifeTime;
}

void Particle::ParticleAssert() {
	assert(drawType != kParticleDrawTypeNull);
	assert(spawnType != kParticleSpawnTypeNull);
	assert(particlesParTime != 0);
	if (drawType == kParticleDrawTypeBox || drawType == kParticleDrawTypeSphere) {
		assert(isChangeFillMode == true);
	}
	if (drawType == kParticleDrawTypeSprite) {
		assert(!(spriteName == ""));
	}
}

void Particle::SetEmitterPos(const Vector2& position) {
	emitter.position = position;
}

void Particle::SetEmittorVelocity(const Vector2& velocity_) {
	emitter.velocity = velocity_;
}

void Particle::SetRandomVelocityRange(float range) {
	randomVelocityRange = range;
}

#ifdef _DEBUG
void Particle::DebugGUI() {
	ImGui::SetNextWindowSize(ImVec2{ 500, 500 }, ImGuiCond_Once);
	ImGui::Begin(std::format("Particle {:p}", (void*)this).c_str(), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
	if (ImGui::Button("Generate")) {
		Generate();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		currentTime = 0;
		particles.clear();
	}
	ImGui::Checkbox("IsLoop", &isLoop);
	int tempSpawn = spawnType;
	ImGui::RadioButton("Sphere", &tempSpawn, 1); ImGui::SameLine();
	ImGui::RadioButton("Box", &tempSpawn, 2); ImGui::SameLine();
	ImGui::RadioButton("Line", &tempSpawn, 3); ImGui::SameLine();
	ImGui::RadioButton("Point", &tempSpawn, 4);
	spawnType = ParticleSpawnType(tempSpawn);
	ImGui::InputFloat2("SpawnRange", &spawnRange.x);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::InputInt("Duration", &duration, 1);
	ImGui::InputInt("Rate", &rate, 1);
	ImGui::InputInt("ParticlesParTime", &particlesParTime, 1);
	ImGui::InputInt("EmitterLifeTime", &emitter.lifeTime, 1);
	ImGui::SliderFloat("RandomLifeTime", &randomLifeTime, 0,1);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::InputFloat2("EmitterVel", &emitter.velocity.x);
	ImGui::SliderFloat("RandomVelRange", &randomVelocityRange, 0, PI);
	ImGui::SliderFloat("RandomVelMag", &randomVelocityMagnification, 0, 1.0f);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::InputFloat2("EmitterAcc", &emitter.acceleration.x);
	ImGui::SliderFloat("RandomAccRange", &randomAccelerarationRange, 0, PI);
	ImGui::SliderFloat("RandomAccMag", &randomAccelerarationMagnification, 0, 1.0f);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::SliderFloat("StartRotate", &emitter.startRotate, 0, 2 * PI);
	ImGui::SliderFloat("RotateSpeed", &emitter.rotateSpeed, 0, 2 * PI);
	ImGui::Checkbox("RotateDireciton", &isRandomizeRotationDirection);
	ImGui::Checkbox("RandomStartRotation", &isRandomizeStartRotation);
	ImGui::SliderFloat("RandomRotateSpeed", &randomRotateSpeed, 0, 1);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::InputFloat2("EmitterSize", &emitter.size.x);
	ImGui::SliderFloat("RandomSize", &randomSize, 0, 1.0f);
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	int tempColor[4] = { int(emitter.color.red), int(emitter.color.green), int(emitter.color.blue), int(emitter.color.alpha) };
	ImGui::SliderInt4("EmitterColor", tempColor, 0,255);
	emitter.color = { unsigned int(tempColor[0]), unsigned int(tempColor[1]), unsigned int(tempColor[2]), unsigned int(tempColor[3]) };
	int tempTargetColor[4] = {int(emitter.targetColor.red), int(emitter.targetColor.green), int(emitter.targetColor.blue), int(emitter.targetColor.alpha)};
	ImGui::SliderInt4("TargetColor", tempTargetColor, 0,255);
	emitter.targetColor = { unsigned int(tempTargetColor[0]), unsigned int(tempTargetColor[1]), unsigned int(tempTargetColor[2]), unsigned int(tempTargetColor[3]) };
	ImGui::End();
}
#endif // _DEBUG

Particle::ParticleProperties::ParticleProperties(const ParticleProperties& emitter, Particle& randomize, const  Vector2& spawnPos) {
	this->position = emitter.position + spawnPos;
	this->velocity = Vector2::Multiply(Vector2::Rotate(emitter.velocity, randomize.random11_float(randomize.engine) * randomize.randomVelocityRange), 1 - randomize.random11_float(randomize.engine) * randomize.randomVelocityMagnification);
	this->acceleration = Vector2::Multiply(Vector2::Rotate(emitter.acceleration, randomize.random11_float(randomize.engine) * randomize.randomAccelerarationRange), 1 - randomize.random11_float(randomize.engine) * randomize.randomAccelerarationMagnification);
	this->size = Vector2::Multiply(emitter.size, 1 - randomize.random11_float(randomize.engine) * randomize.randomSize);
	if (randomize.isRandomizeStartRotation) {
		this->startRotate = randomize.random01_float(randomize.engine) * PI * 2.0f;
	}
	else {
		this->startRotate = emitter.startRotate;
	}
	if (randomize.isRandomizeRotationDirection) {
		this->rotateSpeed = emitter.rotateSpeed * (1 - randomize.random01_float(randomize.engine) * randomize.randomRotateSpeed) * (randomize.random01_int(randomize.engine) * 2 - 1);
	}
	else {
		this->rotateSpeed = emitter.rotateSpeed * (1 - randomize.random01_float(randomize.engine) * randomize.randomRotateSpeed);
	}
	this->color = emitter.color;
	this->lifeTime = int(emitter.lifeTime * (1 - randomize.random11_float(randomize.engine) * randomize.randomLifeTime));
	this->currentTime = 0;
	this->targetColor = emitter.targetColor;
}

void Particle::ParticleProperties::Update() {
	currentTime++;
	position += velocity;
	velocity += acceleration;
}

bool Particle::ParticleProperties::IsErase() {
	return currentTime >= lifeTime ? true : false;
}
