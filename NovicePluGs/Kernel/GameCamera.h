#pragma once

#include "Matrix.h"
#include "Vector2D.h"
#include "Transform2D.h"

#include <memory>

class GameCamera final {
	friend class Debug;
private:// シングルトンなのでprivate
	GameCamera() = default;

public:	// コピー禁止
	GameCamera(const GameCamera&) = delete;
	GameCamera& operator=(const GameCamera&) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

public:
	/// <summary>
	/// カメラ位置の設定
	/// </summary>
	/// <param name="cameraMatrix_"></param>
	static void SetCameraPos(const Vector2& pos);

	static void SetCameraTransform(const Transform2D& transform);

	static void SetNDCPosition(float left, float top, float right, float bottom);

	static void SetViewportInformation(const Vector2& origin, const Vector2& size);
	
	static void SetViewportInformation(Vector2&& origin, Vector2&& size);

	static void Begin();

	static void CameraUpdate();

	static const Matrix3x3& GetVPVMatrix();

	static const Matrix3x3& GetPVMatrix();

	static bool IsRendering(const Vector2& worldVertex);

	static bool IsUpdatedVPMatrix();

	static bool IsUpdatedVPVMatrix();

	static bool IsUpdatedPVMatrix();

private:
	void InstanceCameraUpdate();
	void MakeViewMatrix();
	void MakeOrthoMatrix();
	void MakeViewportMatrix();
	void MakeVPVMatrix();

private:// 一つに絞る
	static std::unique_ptr<GameCamera> instance;

private:// メンバ変数
	Transform2D camera;
	Matrix3x3 viewMatrix;
	Matrix3x3 orthoMatrix;
	Matrix3x3 viewportMatrix;

	Matrix3x3 vpMatrix;
	Matrix3x3 pvMatrix;
	Matrix3x3 vpvMatrix;

	Vector2 cameraTopLeft;
	Vector2 cameraBottomRight;

	Vector2 viewportOrigin;
	Vector2 viewportSize;

	bool isUpdateOthro;
	bool isUpdateViewport;
	bool isUpdateVP;
	bool isUpdateVPV;
	bool isUpdatePV;
};