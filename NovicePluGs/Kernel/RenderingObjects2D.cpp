#include "RenderingObjects2D.h"

#include "GameTextureManager.h"
#include "GameCamera.h"

#include <Novice.h>

void BaseRenderingObjects::clear_all() {
	transform.clear();
	color.clear();
	isRendering.clear();
}

void BaseRenderingObjects::add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) {
	transform.emplace(transform_);
	color.emplace(transform_.get(), color_);
	isRendering.emplace(transform_.get(), std::make_pair<int, int>(1, 0));
}

void BaseRenderingObjects::begin() {
	for (auto itr = isRendering.begin(); itr != isRendering.end(); ++itr) {
		itr->second.second = std::move(itr->second.first);
		itr->second.first = 0;
	}
}

size_t BaseRenderingObjects::registrated_size() {
	return transform.size();
}

Sprite2D::Sprite2D() {
	size = { 0, 0 };
	spriteName = "white1x1";
	init();
}

Sprite2D::Sprite2D(const std::string spriteName_, const Vector2& size_) {
	size = size_;
	spriteName = spriteName_;
	init();
}

void Sprite2D::clear_all() {
	BaseRenderingObjects::clear_all();
	worldVertex.clear();
	renderingBuffer.clear();
}

void Sprite2D::add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) {
	BaseRenderingObjects::add(transform_, color_);
	worldVertex.emplace(transform_.get(), std::vector<Vector2>(0));
	renderingBuffer.emplace(transform_.get(), std::vector<Vector2>(0));
	for (int j = 0; j < localVertex.size(); ++j) {
		worldVertex.at(transform_.get()).emplace_back(Transform2D::Homogeneous(localVertex[j], transform_->get_matrix()));
	}
	for (int j = 0; j < worldVertex.at(transform_.get()).size(); ++j) {
		renderingBuffer.at(transform_.get()).emplace_back(Transform2D::Homogeneous(worldVertex.at(transform_.get())[j], GameCamera::GetVPVMatrix()));
	}
}

void Sprite2D::remove(const std::shared_ptr<Transform2D>& transform_) {
	color.erase(transform_.get());
	isRendering.erase(transform_.get());
	worldVertex.erase(transform_.get());
	renderingBuffer.erase(transform_.get());
	transform.erase(transform_);
}

void Sprite2D::update() {
}

void Sprite2D::begin_rendering() {
	for (auto itr = transform.begin(); itr != transform.end(); ++itr) {
		if ((*itr)->need_update_matrix()) {
			for (int j = 0; j < localVertex.size(); ++j) {
				worldVertex.at(itr->get())[j] = Transform2D::Homogeneous(localVertex[j], (*itr)->get_matrix());
			}
		}
		if (GameCamera::IsUpdatedVPMatrix() || (*itr)->need_update_matrix()) {
			for (int j = 0; j < worldVertex.at(itr->get()).size(); ++j) {
				if (GameCamera::IsRendering(worldVertex.at(itr->get())[j])) {
					isRendering.at(itr->get()).first++;
					break;
				}
			}
		}
		else {
			isRendering.at(itr->get()).first = isRendering.at(itr->get()).second;
		}
		if (isRendering.at(itr->get()).first && (GameCamera::IsUpdatedVPVMatrix() || (*itr)->need_update_matrix())) {
			for (int j = 0; j < worldVertex.at(itr->get()).size(); ++j) {
				renderingBuffer.at(itr->get())[j] = Transform2D::Homogeneous(worldVertex.at(itr->get())[j], GameCamera::GetVPVMatrix());
			}
		}
	}
}

void Sprite2D::draw() const {
	for (auto itr = transform.begin(); itr != transform.end(); ++itr) {
		if (isRendering.at(itr->get()).first) {
			Novice::DrawQuad(
				int(renderingBuffer.at(itr->get())[0].x), int(renderingBuffer.at(itr->get())[0].y),
				int(renderingBuffer.at(itr->get())[1].x), int(renderingBuffer.at(itr->get())[1].y),
				int(renderingBuffer.at(itr->get())[2].x), int(renderingBuffer.at(itr->get())[2].y),
				int(renderingBuffer.at(itr->get())[3].x), int(renderingBuffer.at(itr->get())[3].y),
				0, 0, int(size.x), int(size.y), GameTextureManager::GetHandle(spriteName), color.at(itr->get())->hex()
			);
		}
	}
}

void Sprite2D::debug() {
}

void Sprite2D::init() {
	localVertex = {
		{ -size.x / 2, size.y / 2 },
		{ size.x / 2, size.y / 2 },
		{ -size.x / 2, -size.y / 2 },
		{ size.x / 2, -size.y / 2 }
	};
	worldVertex.clear();
	renderingBuffer.clear();
	begin_rendering();
}

const Vector2& Sprite2D::sprite_size() const {
	return size;
}

const std::string& Sprite2D::sprite_name() const {
	return spriteName;
}

SequentialSprite2D::SequentialSprite2D() {
	this->spriteName = "";
	seq_size = zeroVec2;
	animation = std::make_unique<Animation>(1, 1, false);
	is_frame_forword = false;
	init();
}

SequentialSprite2D::SequentialSprite2D(const std::string spriteName, const Vector2& seq_size_, std::unique_ptr<Animation> animation_, bool is_frame_forword_) {
	this->spriteName = spriteName;
	seq_size = seq_size_;
	animation = std::move(animation_);
	is_frame_forword = is_frame_forword_;
	init();
}

void SequentialSprite2D::clear_all() {
	BaseRenderingObjects::clear_all();
	worldVertex.clear();
	renderingBuffer.clear();
	currentTime.clear();
}

void SequentialSprite2D::add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) {
	SequentialSprite2D::add(transform_, color_, 0);
}

void SequentialSprite2D::add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_, int currentTime_) {
	BaseRenderingObjects::add(transform_, color_);
	currentTime.emplace(transform_.get(), currentTime_);
	worldVertex.emplace(transform_.get(), std::vector<Vector2>(0));
	renderingBuffer.emplace(transform_.get(), std::vector<Vector2>(0));
	for (int j = 0; j < localVertex.size(); ++j) {
		worldVertex.at(transform_.get()).emplace_back(Transform2D::Homogeneous(localVertex[j], transform_->get_matrix()));
	}
	for (int j = 0; j < worldVertex.at(transform_.get()).size(); ++j) {
		renderingBuffer.at(transform_.get()).emplace_back(Transform2D::Homogeneous(worldVertex.at(transform_.get())[j], GameCamera::GetVPVMatrix()));
	}
}

void SequentialSprite2D::remove(const std::shared_ptr<Transform2D>& transform_) {
	color.erase(transform_.get());
	isRendering.erase(transform_.get());
	worldVertex.erase(transform_.get());
	renderingBuffer.erase(transform_.get());
	transform.erase(transform_);
}

void SequentialSprite2D::begin() {
	if (is_frame_forword) {
		for (auto itr = currentTime.begin(); itr != currentTime.end(); ++itr) {
			++(*itr).second;
		}
	}
	BaseRenderingObjects::begin();
}

void SequentialSprite2D::update() {
}

void SequentialSprite2D::begin_rendering() {
	for (auto itr = transform.begin(); itr != transform.end(); ++itr) {
		if ((*itr)->need_update_matrix()) {
			for (int j = 0; j < localVertex.size(); ++j) {
				worldVertex.at(itr->get())[j] = Transform2D::Homogeneous(localVertex[j], (*itr)->get_matrix());
			}
		}
		if (GameCamera::IsUpdatedVPMatrix() || (*itr)->need_update_matrix()) {
			for (int j = 0; j < worldVertex.at(itr->get()).size(); ++j) {
				if (GameCamera::IsRendering(worldVertex.at(itr->get())[j])) {
					isRendering.at(itr->get()).first++;
					break;
				}
			}
		}
		else {
			isRendering.at(itr->get()).first = isRendering.at(itr->get()).second;
		}
		if (isRendering.at(itr->get()).first && (GameCamera::IsUpdatedVPVMatrix() || (*itr)->need_update_matrix())) {
			for (int j = 0; j < worldVertex.at(itr->get()).size(); ++j) {
				renderingBuffer.at(itr->get())[j] = Transform2D::Homogeneous(worldVertex.at(itr->get())[j], GameCamera::GetVPVMatrix());
			}
		}
	}
}

void SequentialSprite2D::draw() const {
	int now_seq;
	for (auto itr = transform.begin(); itr != transform.end(); ++itr) {
		if (isRendering.at(itr->get()).first) {
			now_seq = animation->now(currentTime.at(itr->get()));
			Novice::DrawQuad(
				int(renderingBuffer.at(itr->get())[0].x), int(renderingBuffer.at(itr->get())[0].y),
				int(renderingBuffer.at(itr->get())[1].x), int(renderingBuffer.at(itr->get())[1].y),
				int(renderingBuffer.at(itr->get())[2].x), int(renderingBuffer.at(itr->get())[2].y),
				int(renderingBuffer.at(itr->get())[3].x), int(renderingBuffer.at(itr->get())[3].y),
				int(seq_size.x * now_seq), 0, int(seq_size.x), int(seq_size.y), GameTextureManager::GetHandle(spriteName), color.at(itr->get())->hex()
			);
		}
	}
}

void SequentialSprite2D::debug() {
}

void SequentialSprite2D::init() {
	localVertex = {
		{ -seq_size.x / 2, seq_size.y / 2 },
		{ seq_size.x / 2, seq_size.y / 2 },
		{ -seq_size.x / 2, -seq_size.y / 2 },
		{ seq_size.x / 2, -seq_size.y / 2 }
	};
	worldVertex.clear();
	renderingBuffer.clear();
	currentTime.clear();
	begin_rendering();
}

const Vector2& SequentialSprite2D::seq_sprite_size() {
	return seq_size;
}

void SequentialSprite2D::set_current(const std::shared_ptr<Transform2D>& transform_, int time) {
	currentTime.at(transform_.get()) = time;
}

CameraSprite2D::CameraSprite2D() {
	size = zeroVec2;
	spriteName = "";
	init();
}

CameraSprite2D::CameraSprite2D(const std::string spriteName_, const Vector2& size_) {
	size = size_;
	spriteName = spriteName_;
	init();
}

void CameraSprite2D::add(const std::shared_ptr<Transform2D>& transform_, const std::shared_ptr<Color>& color_) {
	BaseRenderingObjects::add(transform_, color_);
	screenVertex.emplace(transform_.get(), std::vector<Vector2>(0));
	for (int i = 0; i < localVertex.size(); ++i) {
		screenVertex.at(transform_.get()).push_back(Transform2D::Homogeneous(localVertex[i], transform_->get_matrix()));
	}
}

void CameraSprite2D::remove(const std::shared_ptr<Transform2D>& transform_) {
	color.erase(transform_.get());
	isRendering.erase(transform_.get());
	screenVertex.erase(transform_.get());
	transform.erase(transform_);
}

void CameraSprite2D::update() {
}

void CameraSprite2D::begin_rendering() {
	for (auto itr = transform.begin(); itr != transform.end(); ++itr) {
		if ((*itr)->need_update_matrix()) {
			for (int j = 0; j < localVertex.size(); ++j) {
				screenVertex.at(itr->get())[j] = Transform2D::Homogeneous(localVertex[j], (*itr)->get_matrix());
			}
		}
	}
}

void CameraSprite2D::draw() const {
	for (auto itr = transform.begin(); itr != transform.end(); ++itr) {
		Novice::DrawQuad(
			int(screenVertex.at(itr->get())[0].x), int(screenVertex.at(itr->get())[0].y),
			int(screenVertex.at(itr->get())[1].x), int(screenVertex.at(itr->get())[1].y),
			int(screenVertex.at(itr->get())[2].x), int(screenVertex.at(itr->get())[2].y),
			int(screenVertex.at(itr->get())[3].x), int(screenVertex.at(itr->get())[3].y),
			0, 0, int(size.x), int(size.y), GameTextureManager::GetHandle(spriteName), color.at(itr->get())->hex()
		);
	}
}

void CameraSprite2D::debug() {
}

void CameraSprite2D::init() {
	localVertex = {
		{ -size.x / 2, -size.y / 2 },
		{ size.x / 2, -size.y / 2 },
		{ -size.x / 2, size.y / 2 },
		{ size.x / 2, size.y / 2 }
	};
	screenVertex.clear();
	begin_rendering();
}
