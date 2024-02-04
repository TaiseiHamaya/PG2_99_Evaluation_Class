#include "Collision.h"

#include "NovicePluGs.h"

bool Collision::BoxToBox(
	const Vector2& pos1, const Vector2& radius1, const Vector2& pos2, const Vector2& radius2
) {
	return
		pos1.x - radius1.x < pos2.x + radius2.x && pos1.x + radius1.x > pos2.x - radius2.x &&
		pos1.y - radius1.y < pos2.y + radius2.y && pos1.y + radius1.y > pos2.y - radius2.y;
}

bool Collision::RectToRect(const Rect& rect1, const Rect& rect2) {
	return RectToLine(rect1, rect2.topLeft, rect2.topRight) ||
		RectToLine(rect1, rect2.topRight, rect2.bottomRight) ||
		RectToLine(rect1, rect2.bottomRight, rect2.bottomLeft) ||
		RectToLine(rect1, rect2.bottomLeft, rect2.topLeft) ||
		RectToLine(rect2, rect1.topLeft, rect1.topRight) ||
		RectToLine(rect2, rect1.topRight, rect1.bottomRight) ||
		RectToLine(rect2, rect1.bottomRight, rect1.bottomLeft) ||
		RectToLine(rect2, rect1.bottomLeft, rect1.topLeft);
}

bool Collision::SphereToSphere(const Vector2& pos1, const float& radius1, const Vector2& pos2, const float& radius2) {
	return (pos1 - pos2).length() < radius1 + radius2;
}

bool Collision::LineToLine(const Vector2& initialPoint1, const Vector2& terminalPoint1, const Vector2& initialPoint2, const Vector2& terminalPoint2) {
	Vector2 base1 = terminalPoint1 - initialPoint1;
	Vector2 base2 = terminalPoint2 - initialPoint2;
	return Vector2::CrossProduct(initialPoint2 - initialPoint1, base1) * Vector2::CrossProduct(terminalPoint2 - initialPoint1, base1) < 0 && Vector2::CrossProduct(initialPoint1 - initialPoint2, base2) * Vector2::CrossProduct(terminalPoint1 - initialPoint2, base2) < 0;
}

bool Collision::RectToLine(const Rect& rect, const Vector2& initialPoint, const Vector2& terminalPoint) {
	return RectToDot(rect, initialPoint) || RectToDot(rect, terminalPoint) ||
		LineToLine(rect.topLeft, rect.topRight, initialPoint, terminalPoint) ||
		LineToLine(rect.topRight, rect.bottomRight, initialPoint, terminalPoint) ||
		LineToLine(rect.bottomRight, rect.bottomLeft, initialPoint, terminalPoint) ||
		LineToLine(rect.bottomLeft, rect.topLeft, initialPoint, terminalPoint);
}

bool Collision::RectToDot(const Rect& rect, const Vector2& point) {
	return
		Vector2::CrossProduct(rect.topRight - rect.topLeft, point - rect.topLeft) <= 0 &&
		Vector2::CrossProduct(rect.bottomRight - rect.topRight, point - rect.topRight) <= 0 &&
		Vector2::CrossProduct(rect.bottomLeft - rect.bottomRight, point - rect.bottomRight) <= 0 &&
		Vector2::CrossProduct(rect.topLeft - rect.bottomLeft, point - rect.bottomLeft) <= 0;
}

bool Collision::LineToSphere(const Vector2& initialPoint, const Vector2& terminalPoint, const Vector2& spherePos, const float& radius) {
	float t;
	radius;
	t = Vector2::DotProduct((terminalPoint - initialPoint).normalize(), spherePos);
	//Vector2 s = Lerp(initialPoint, terminalPoint, t);
	//if ()
	return false;
}
