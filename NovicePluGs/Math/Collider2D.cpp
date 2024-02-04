#include "Collider2D.h"

#include "Transform2D.h"
#include "Renderer.h"
#include "GameCamera.h"
#include "Easing.h"

#include <algorithm>

BaseCollider::BaseCollider() {
	colliderType = kColliderTypeStatic;
}

AABB2D::AABB2D() : BaseCollider() {
	transform = std::make_shared<Transform2D>();
	transform->set_translate(zeroVec2);
	init(kColliderTypeStatic, zeroVec2);
}

AABB2D::AABB2D(ColliderType colliderType_, const Vector2& position, const Vector2& size) {
	transform = std::make_shared<Transform2D>();
	transform->set_translate(position);
	init(colliderType_, size);
}

AABB2D::AABB2D(ColliderType colliderType_, std::shared_ptr<Transform2D>& transform_, const Vector2& size) {
	transform = transform_;
	init(colliderType_, size);
}

void AABB2D::begin() {
}

void AABB2D::update() {
	if (colliderType != kColliderTypeStatic) {
		*matrix = Transform2D::MakeTranslateMatrix(transform->get_translate());
		for (int i = 0; i < localVertex.size(); ++i) {
			worldVertex[i] = Transform2D::Homogeneous(localVertex[i], *matrix);
		}
	}
}

void AABB2D::debug() {
#ifdef _DEBUG

	std::vector<Vector2> tempVertex{ worldVertex.size() };
	for (int i = 0; i < worldVertex.size(); ++i) {
		tempVertex[i] = Transform2D::Homogeneous(worldVertex[i], GameCamera::GetVPVMatrix());
	}
	Renderer::DrawLine(tempVertex[0], tempVertex[1], WHITE);
	Renderer::DrawLine(tempVertex[1], tempVertex[3], WHITE);
	Renderer::DrawLine(tempVertex[3], tempVertex[2], WHITE);
	Renderer::DrawLine(tempVertex[2], tempVertex[0], WHITE);

#endif // _DEBUG
}

void AABB2D::init(ColliderType colliderType_, const Vector2& size) {
	localVertex = {
		{ -size.x / 2, size.y / 2 },
		{ size.x / 2, size.y / 2 },
		{ -size.x / 2, -size.y / 2 },
		{ size.x / 2, -size.y / 2 }
	};
	worldVertex.resize(localVertex.size());
	colliderType = colliderType_;
	matrix.reset(new Matrix3x3{ Transform2D::MakeTranslateMatrix(transform->get_translate()) });
	for (int i = 0; i < localVertex.size(); ++i) {
		worldVertex[i] = Transform2D::Homogeneous(localVertex[i], *matrix);
	}
}

Circle::Circle(ColliderType colliderType_, const Vector2& position, float radius) {
	transform = std::make_shared<Transform2D>();
	transform->set_translate(position);
	this->radius = radius;
	colliderType = colliderType_;
}

Circle::Circle(ColliderType colliderType_, std::shared_ptr<Transform2D>& transform_, float radius) {
	transform = transform_;
	this->radius = radius;
	colliderType = colliderType_;
}

void Circle::begin() {
}

void Circle::update() {
}

void Circle::debug() {
	Vector2 tempPos = Transform2D::Homogeneous(transform->get_translate(), GameCamera::GetVPVMatrix());
	Novice::DrawEllipse(int(tempPos.x), int(tempPos.y), int(radius), int(radius), 0, WHITE, kFillModeWireFrame);
}

Capcel::Capcel(ColliderType colliderType_, const Vector2& position, float radius) {
	transform = std::make_shared<Transform2D>();
	transform->set_translate(position);
	this->radius = radius;
	colliderType = colliderType_;
	prePos = transform->get_translate();
}

Capcel::Capcel(ColliderType colliderType_, std::shared_ptr<Transform2D>& transform_, float radius) {
	transform = transform_;
	this->radius = radius;
	colliderType = colliderType_;
	prePos = transform->get_translate();
}

void Capcel::begin() {
	prePos = transform->get_translate();
}

void Capcel::update() {
}

void Capcel::debug() {
	Vector2 terminal = Transform2D::Homogeneous(transform->get_translate(), GameCamera::GetVPVMatrix());
	Vector2 initial = Transform2D::Homogeneous(prePos, GameCamera::GetVPVMatrix());

	Novice::DrawEllipse(int(terminal.x), int(terminal.y), int(radius), int(radius), 0, WHITE, kFillModeWireFrame);
	Novice::DrawEllipse(int(initial.x), int(initial.y), int(radius), int(radius), 0, WHITE, kFillModeWireFrame);
	Vector2 v = terminal != initial ?  (terminal - initial).normalize() :  zeroVec2;
	Vector2 w_v1 = Vector2::Rotate(v, -1, 0) * radius;
	
	Renderer::DrawLine(terminal + w_v1, initial + w_v1, WHITE);
	Renderer::DrawLine(terminal - w_v1, initial - w_v1, WHITE);
}

bool Collider::CircleToCapcel(const Circle& circle, const Capcel& capcel) {
	Vector2 mid;
	if (capcel.transform->get_translate() == capcel.prePos) {
		mid = capcel.transform->get_translate();
	}
	else if (circle.transform->get_translate() == capcel.transform->get_translate()) {
		return true;
	}else{
		Vector2 terget = circle.transform->get_translate() - capcel.transform->get_translate();
		Vector2 norm = (capcel.prePos - capcel.transform->get_translate()).normalize();
		float length = (capcel.prePos - capcel.transform->get_translate()).length();
		float t = std::clamp(Vector2::DotProduct(terget, norm) / length, 0.0f, 1.0f);
		mid = Lerp(capcel.transform->get_translate(), capcel.prePos, t);
	}
	return (circle.transform->get_translate() - mid).length() <= circle.radius + capcel.radius;
}

bool Collider::CircleToCircle(const Circle& circle1, const Circle& circle2) {
	Vector2 distancce = circle1.transform->get_translate() - circle2.transform->get_translate();
	return distancce.length() <= circle1.radius + circle2.radius;
}

bool Collider::AABB2DToDot(const AABB2D& aabb, const Vector2& position) {
	return aabb.worldVertex[0].x <= position.x && position.x <= aabb.worldVertex[1].x &&
		aabb.worldVertex[2].y <= position.y && position.y <= aabb.worldVertex[0].y;
}
