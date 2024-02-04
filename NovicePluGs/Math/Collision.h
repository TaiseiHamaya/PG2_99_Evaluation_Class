#pragma once

#include "Vector2D.h"
#include "Definition.h"

class Collision final {
public:
	/// <summary>
	/// 箱同士のコリジョン
	/// <param name="pos1">箱の位置1</param>
	/// <param name="radius1">箱の大きさ1</param>
	/// <param name="pos2">箱の位置2</param>
	/// <param name="radius2">箱の大きさ2</param>
	/// </summary>
	static bool BoxToBox(const Vector2& pos1, const Vector2& radius1, const Vector2& pos2, const Vector2& radius2);
	
	/// <summary>
	/// 長方形同士のコリジョン
	/// </summary>
	/// <param name="rect1">長方形1</param>
	/// <param name="rect2">長方形2</param>
	/// <returns></returns>
	static bool RectToRect(const Rect& rect1, const Rect& rect2);

	/// <summary>
	/// 円同士のコリジョン
	/// </summary>
	/// <param name="pos1">円1のpos</param>
	/// <param name="radius1">円1の半径</param>
	/// <param name="pos2">円2のpos</param>
	/// <param name="radius2">円2の半径</param>
	/// <returns></returns>
	static bool SphereToSphere(const Vector2& pos1, const float& radius1, const Vector2& pos2, const float& radius2);

	/// <summary>
	/// 直線同士のコリジョン
	/// </summary>
	/// <param name="linePos1"></param>
	/// <param name="linePos2"></param>
	/// <returns></returns>
	static bool LineToLine(const Vector2& initialPoint1, const Vector2& terminalPoint1, const Vector2& initialPoint2, const Vector2& terminalPoint2);
	
	/// <summary>
	/// 長方形と線の当たり判定
	/// </summary>
	/// <param name="rect">長方形</param>
	/// <param name="initialPoint">線の始点</param>
	/// <param name="terminalPoint">線の終点</param>
	/// <returns></returns>
	static bool RectToLine(const Rect& rect ,const Vector2& initialPoint, const Vector2& terminalPoint);
	
	/// <summary>
	/// 長方形と点の当たり判定
	/// </summary>
	/// <param name="rect">長方形</param>
	/// <param name="point">点</param>
	/// <returns></returns>
	static bool RectToDot(const Rect& rect ,const Vector2& point);

	/// <summary>
	/// カプセル同士のコリジョン
	/// </summary>
	/// <returns></returns>
	static bool CapsuleToCapsule();

	/// <summary>
	/// 直線と円のコリジョン(未定義)
	/// </summary>
	/// <param name="initialPoint">直線の始点</param>
	/// <param name="terminalPoint">直線の終点</param>
	/// <param name="spherePos">円の位置</param>
	/// <param name="radius">円の半径</param>
	/// <returns></returns>
	static bool LineToSphere(const Vector2& initialPoint, const Vector2& terminalPoint, const Vector2& spherePos, const float& radius);

};