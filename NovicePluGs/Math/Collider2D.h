#pragma once

#include <vector>
#include <memory>

#include "Transform2D.h"

enum ColliderType {
	kColliderTypeArea,
	kColliderTypeStatic,
	kColliderTypeRigid,
	kColliderTypeKinematic,
};

class BaseCollider {
public:
	BaseCollider();

public:
	virtual void begin() = 0;
	virtual void update() = 0;
	virtual void debug() = 0;

public:
	ColliderType colliderType;
	std::shared_ptr<Transform2D> transform;
};

class AABB2D : public BaseCollider {
public:
	AABB2D();
	AABB2D(ColliderType colliderType_, const Vector2& position = zeroVec2, const Vector2& size = zeroVec2);
	AABB2D(ColliderType colliderType_, std::shared_ptr<Transform2D>& transform_, const Vector2& size = zeroVec2);
	~AABB2D() = default;

public:
	void begin() override;
	void update() override;
	void debug() override;

private:
	void init(ColliderType colliderType_, const Vector2& size);

public:
	std::unique_ptr<Matrix3x3> matrix;
	std::vector<Vector2> localVertex;
	std::vector<Vector2> worldVertex;
};

class Circle : public BaseCollider {
public:
	Circle() = default;	
	Circle(ColliderType colliderType_, const Vector2& position = zeroVec2, float radius = 0);
	Circle(ColliderType colliderType_, std::shared_ptr<Transform2D>& transform_, float radius = 0);

public:
	void begin() override;
	void update() override;
	void debug() override;

public:
	float radius;
};

class Capcel : public BaseCollider {
public:
	Capcel() = default;
	Capcel(ColliderType colliderType_, const Vector2& position = zeroVec2, float radius = 0);
	Capcel(ColliderType colliderType_, std::shared_ptr<Transform2D>& transform_, float radius = 0);

public:
	void begin() override;
	void update() override;
	void debug() override;

public:
	Vector2 prePos;
	float radius;
};

namespace Collider {
	extern bool CircleToCapcel(const Circle& circle, const Capcel& capcel);
	extern bool CircleToCircle(const Circle& circle1, const Circle& circle2);
	extern bool AABB2DToDot(const AABB2D& aabb, const Vector2& position);
}