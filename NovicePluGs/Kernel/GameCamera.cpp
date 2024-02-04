#include "GameCamera.h"

std::unique_ptr<GameCamera> GameCamera::instance;

void GameCamera::Initialize() {
	instance.reset(new GameCamera{});
	instance->SetNDCPosition(0, 0, 0, 0);
	instance->SetViewportInformation({ 0, 0 }, { 0, 0 });
	instance->InstanceCameraUpdate();
	instance->isUpdateOthro = true;
	instance->isUpdateViewport = true;
	instance->isUpdatePV = true;
	instance->isUpdateVPV = true;
}

void GameCamera::SetCameraPos(const Vector2& pos) {
	instance->camera.set_translate(pos);
}

void GameCamera::SetCameraTransform(const Transform2D& transform) {
	instance->camera = transform;
}

void GameCamera::SetNDCPosition(float left, float top, float right, float bottom) {
	if (instance->cameraTopLeft.x != left || instance->cameraTopLeft.y != top ||
		instance->cameraBottomRight.x != right || instance->cameraBottomRight.y != bottom) {
		instance->cameraTopLeft = { left, top };
		instance->cameraBottomRight = { right, bottom };
		instance->isUpdateOthro = true;
	}
}

void GameCamera::SetViewportInformation(const Vector2& origin, const Vector2& size) {
	if (instance->viewportOrigin != origin || instance->viewportSize != size) {
		instance->viewportOrigin = origin;
		instance->viewportSize = size;
		instance->isUpdateViewport = true;
	}
}

void GameCamera::SetViewportInformation(Vector2&& origin, Vector2&& size) {
	if (instance->viewportOrigin != origin || instance->viewportSize != size) {
		instance->viewportOrigin = origin;
		instance->viewportSize = size;
		instance->isUpdateViewport = true;
	}
}

void GameCamera::Begin() {
	instance->isUpdateOthro = false;
	instance->isUpdateViewport = false;
	instance->isUpdateVPV = false;
	instance->isUpdateVP = false;
	instance->isUpdatePV = false;
	instance->camera.begin();
}

void GameCamera::CameraUpdate() {
	instance->InstanceCameraUpdate();
}

const Matrix3x3& GameCamera::GetVPVMatrix() {
	return instance->vpvMatrix;
}

const Matrix3x3& GameCamera::GetPVMatrix() {
	return instance->pvMatrix;
}

bool GameCamera::IsRendering(const Vector2& worldVertex) {
	Vector2 temp = Transform2D::Homogeneous(worldVertex, instance->vpMatrix);
	return (temp.x >= -1 && temp.x <= 1 && temp.y >= -1 && temp.y <= 1);
}

bool GameCamera::IsUpdatedVPMatrix() {
    return instance->isUpdateVP;
}

bool GameCamera::IsUpdatedVPVMatrix() {
	return instance->isUpdateVPV;
}

bool GameCamera::IsUpdatedPVMatrix() {
	return instance->isUpdatePV;
}

void GameCamera::InstanceCameraUpdate() {
	camera.update();
	MakeViewMatrix();
	MakeOrthoMatrix();
	MakeViewportMatrix();
	if (camera.need_update_matrix() || isUpdateOthro) {
		vpMatrix = viewMatrix * orthoMatrix;
		isUpdateVP = true;
	}
	if (isUpdateOthro || isUpdateViewport) {
		pvMatrix = orthoMatrix * viewportMatrix;
		isUpdatePV = true;
	}
	MakeVPVMatrix();
}

void GameCamera::MakeViewMatrix() {
	if (camera.need_update_matrix()) {
		viewMatrix = camera.get_matrix().inverse();
	}
}

void GameCamera::MakeOrthoMatrix() {
	if (isUpdateOthro) {
		orthoMatrix = { {
			{ 2.0f / (cameraBottomRight.x - cameraTopLeft.x), 0, 0 },
			{ 0, 2.0f / (cameraTopLeft.y - cameraBottomRight.y), 0 },
			{ (cameraTopLeft.x + cameraBottomRight.x) / (cameraTopLeft.x - cameraBottomRight.x), (cameraBottomRight.y + cameraTopLeft.y) / (cameraBottomRight.y - cameraTopLeft.y), 1 } }
		};
	}
}

void GameCamera::MakeViewportMatrix() {
	if (isUpdateViewport) {
		viewportMatrix = { {
			{ viewportSize.x / 2, 0, 0 },
			{ 0, -viewportSize.y / 2, 0 },
			{ viewportOrigin.x + viewportSize.x / 2, viewportOrigin.y + viewportSize.y / 2, 1 } }
		};
	}
}

void GameCamera::MakeVPVMatrix() {
	if (camera.need_update_matrix() || isUpdateOthro || isUpdateViewport) {
		vpvMatrix = vpMatrix * viewportMatrix;
		isUpdateVPV = true;
	}
}
