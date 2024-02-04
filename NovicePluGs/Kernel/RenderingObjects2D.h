#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "Color.h"
#include "Transform2D.h"

class BaseRenderingObjects {
public:
	BaseRenderingObjects() = default;
	virtual ~BaseRenderingObjects() = default;

public:
	virtual void clear_all();
	virtual void add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_);
	virtual void begin();
	virtual void update() = 0;
	virtual void begin_rendering() = 0;
	virtual void draw() const = 0;
	virtual void debug() = 0;

public:
	size_t registrated_size();

public:
	std::unordered_set<std::shared_ptr<Transform2D>> transform;
	std::unordered_map<Transform2D*, std::shared_ptr<Color>> color;

protected:
	std::unordered_map<Transform2D*, std::pair<int, int>> isRendering;
};

class Sprite2D : public BaseRenderingObjects {
public:
	Sprite2D();
	Sprite2D(const std::string spriteName, const Vector2& size_);
	~Sprite2D() = default;

public:
	void clear_all() override;
	void add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) override;
	void remove(const std::shared_ptr<Transform2D>& transform_);
	void update();
	void begin_rendering() override;
	void draw() const override;
	void debug() override;

private:
	void init();

public:
	const Vector2& sprite_size() const;
	const std::string& sprite_name() const;

private:
	std::string spriteName;
	Vector2 size;
	std::vector<Vector2> localVertex;
	std::unordered_map<Transform2D*, std::vector<Vector2>> worldVertex;

	std::unordered_map<Transform2D*, std::vector<Vector2>> renderingBuffer;
};

class SequentialSprite2D : public BaseRenderingObjects {
public:
	SequentialSprite2D();
	SequentialSprite2D(const std::string spriteName, const Vector2& seq_size_, std::unique_ptr<Animation> animation_, bool is_frame_forword);
	~SequentialSprite2D() = default;

public:
	void clear_all() override;
	void add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) override;
	void add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_, int currentTime_);
	void remove(const std::shared_ptr<Transform2D>& transform_);
	void begin() override;
	void update() override;
	void begin_rendering() override;
	void draw() const override;
	void debug() override;

private:
	void init();

public:
	const Vector2& seq_sprite_size();
	void set_current(const std::shared_ptr<Transform2D>& transform_, int time);

private:
	std::unordered_map<Transform2D*, int> currentTime;
	std::string spriteName;
	Vector2 seq_size;
	std::vector<Vector2> localVertex;
	std::unordered_map<Transform2D*, std::vector<Vector2>> worldVertex;

	std::unordered_map<Transform2D*, std::vector<Vector2>> renderingBuffer;

	std::unique_ptr<Animation> animation;

	bool is_frame_forword;
};

class CameraSprite2D : public BaseRenderingObjects {
public:
	CameraSprite2D();
	CameraSprite2D(const std::string spriteName_, const Vector2& size_);
	~CameraSprite2D() = default;

public:
	void add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) override;
	void remove(const std::shared_ptr<Transform2D>& transform_);
	void update() override;
	void begin_rendering() override;
	void draw() const override;
	void debug() override;

private:
	void init();

public:
	const Vector2& sprite_size();

private:
	std::string spriteName;
	Vector2 size;
	std::vector<Vector2> localVertex;
	std::unordered_map<Transform2D*, std::vector<Vector2>> screenVertex;
};
