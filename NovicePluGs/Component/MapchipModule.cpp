#include "MapchipModule.h"

#include "NovicePluGs.h"
#include "Renderer.h"
#include "GameTextureManager.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

void Mapchip::Initialize(int mapchipSize_, const std::string& spriteFileDir, const std::vector<MapchipObjectData>& objectData_) {
	mapchipSize = mapchipSize_;
	GameTextureManager::Register("Mapchip", spriteFileDir);
	objectData = objectData_;
	mapchipData.clear();
	sprite = std::make_unique<SequentialSprite2D>("Mapchip", Vector2{ float(mapchipSize),float(mapchipSize) }, std::make_unique<Animation>(1, 1, false), false);
}

void Mapchip::LoadMapchipdata(const std::string& fileDire, bool isReverseRow, bool isReverseColumn) {
	std::ifstream file(fileDire);

	if (file.fail()) {
		throw std::runtime_error("Failed to open file"); // ファイルが無い場合
	}
	mapchipData.clear();
	std::string lineBuf;
	while (std::getline(file, lineBuf, '\n')) {
		mapchipData.resize(mapchipData.size() + 1);
		std::string charBuf;
		std::stringstream lineTemp(lineBuf);
		while (std::getline(lineTemp, charBuf, ',')) {
			mapchipData.back().emplace_back(std::stoi(charBuf), nullptr);
		}
	}
	row = mapchipData.size();
	column = mapchipData[0].size();
	if (isReverseRow) {
		std::reverse(mapchipData.begin(), mapchipData.end());
	}
	if (isReverseColumn) {
		for (int i = 0; i < mapchipData.size(); i++) {
			std::reverse(mapchipData[i].begin(), mapchipData[i].end());
		}
	}
	for (int i = 0; i < mapchipData.size(); i++) {
		for (int j = 0; j < mapchipData[i].size(); j++) {
			mapchipData[i][j].transform = std::make_shared<Transform2D>(Vector2{ 1.0f,1.0f }, 0.0f, Vector2{ j * mapchipSize + mapchipSize / 2.0f, i * mapchipSize + mapchipSize / 2.0f });
			sprite->add(mapchipData[i][j].transform, std::make_shared<Color>(0xFFFFFFFF), mapchipData[i][j].data);
		}
	}
}

void Mapchip::Begin() {
	for (int i = 0; i < mapchipData.size(); i++) {
		for (int j = 0; j < mapchipData[i].size(); j++) {
			mapchipData[i][j].transform->begin();
		}
	}
	sprite->begin();
}

void Mapchip::BeginRendering() {
	for (int i = 0; i < mapchipData.size(); i++) {
		for (int j = 0; j < mapchipData[i].size(); j++) {
			mapchipData[i][j].transform->update();
		}
	}
	sprite->update();
	sprite->begin_rendering();
}

void Mapchip::Draw() const {
	sprite->draw();
}

void Mapchip::Debug(const Matrix3x3& camera) {
	Novice::ScreenPrintf(420, 0, "topLeftPos[%d][%d] = %d", int(topLeft.y / mapchipSize), int(topLeft.x / mapchipSize), topLeftMapchip);
	Novice::ScreenPrintf(650, 0, "topRightPos[%d][%d] = %d", int(topRight.y / mapchipSize), int(topRight.x / mapchipSize), topRightMapchip);
	Novice::ScreenPrintf(420, 20, "bottomLeftPos[%d][%d] = %d", int(bottomLeft.y / mapchipSize), int(bottomLeft.x / mapchipSize), bottomLeftMapchip);
	Novice::ScreenPrintf(650, 20, "bottomRightPos[%d][%d] = %d", int(bottomRight.y / mapchipSize), int(bottomRight.x / mapchipSize), bottomRightMapchip);

	for (int i = 0; i < row; i++) {
		Vector2 internal = Transform2D::Homogeneous({ 0, float(i * mapchipSize) }, camera);
		Vector2 terminal = Transform2D::Homogeneous({ float(column * mapchipSize), float(i * mapchipSize) }, camera);
		Renderer::DrawLine(internal, terminal, 0xff00ff80);
	}
	for (int i = 0; i < column; i++) {
		Vector2 internal = Transform2D::Homogeneous({ float(i * mapchipSize), 0 }, camera);
		Vector2 terminal = Transform2D::Homogeneous({ float(i * mapchipSize), float(row * mapchipSize) }, camera);
		Renderer::DrawLine(internal, terminal, 0xff00ff80);
	}
}

bool Mapchip::IsBoxInclueBlockName(const std::string& blockName, const Vector2& pos, const Vector2& radius, const Vector2& velocity) {
	topLeft = { pos.x - radius.x + velocity.x, pos.y - radius.y + velocity.y };
	topRight = { pos.x + radius.x - 0.1f + velocity.x, pos.y - radius.y + velocity.y };
	bottomLeft = { pos.x - radius.x + velocity.x, pos.y + radius.y - 0.1f + velocity.y };
	bottomRight = { pos.x + radius.x - 0.1f + velocity.x, pos.y + radius.y - 0.1f + velocity.y };

	topLeftMapchip = mapchipData[int(topLeft.y / mapchipSize)][int(topLeft.x / mapchipSize)].data;
	topRightMapchip = mapchipData[int(topRight.y / mapchipSize)][int(topRight.x / mapchipSize)].data;
	bottomLeftMapchip = mapchipData[int(bottomLeft.y / mapchipSize)][int(bottomLeft.x / mapchipSize)].data;
	bottomRightMapchip = mapchipData[int(bottomRight.y / mapchipSize)][int(bottomRight.x / mapchipSize)].data;

	bool result = false;
	if (objectData[topLeftMapchip].name == blockName) {
		result = true;
	}
	if (objectData[topRightMapchip].name == blockName) {
		result = true;
	}

	if (objectData[bottomLeftMapchip].name == blockName) {
		result = true;
	}
	if (objectData[bottomRightMapchip].name == blockName) {
		result = true;
	}
	return result;
}

void Mapchip::Collision(std::shared_ptr<Transform2D>& transform, const Vector2& radius, Vector2* velocity) {
	topLeft = { transform->get_translate().x - radius.x + velocity->x, transform->get_translate().y - radius.y + velocity->y};
	topRight = { transform->get_translate().x + radius.x + velocity->x - 0.1f, transform->get_translate().y - radius.y + velocity->y };
	bottomLeft = { transform->get_translate().x - radius.x + velocity->x, transform->get_translate().y + radius.y + velocity->y - 0.1f };
	bottomRight = { transform->get_translate().x + radius.x + velocity->x - 0.1f, transform->get_translate().y + radius.y + velocity->y - 0.1f };

	topLeftMapchip = mapchipData[int(topLeft.y / mapchipSize)][int(topLeft.x / mapchipSize)].data;
	topRightMapchip = mapchipData[int(topRight.y / mapchipSize)][int(topRight.x / mapchipSize)].data;
	bottomLeftMapchip = mapchipData[int(bottomLeft.y / mapchipSize)][int(bottomLeft.x / mapchipSize)].data;
	bottomRightMapchip = mapchipData[int(bottomRight.y / mapchipSize)][int(bottomRight.x / mapchipSize)].data;

	if (velocity->x == 0 || velocity->y == 0) {
		// 何も操作しないとき
		if (velocity->x == 0 && velocity->y == 0) {
			// すべて壁である
			if (objectData[topLeftMapchip].isCollision && objectData[topRightMapchip].isCollision && objectData[bottomLeftMapchip].isCollision && objectData[bottomRightMapchip].isCollision) {
				return;
			}
			// 左上
			if (objectData[topLeftMapchip].isCollision) {
				if (Vector2::CrossProduct(Vector2{ -1, -1 }.normalize(), (topLeft - Vector2{ float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize), float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) }).normalize()) > 0) {
					transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) + radius.x);
				}
				else {
					transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) + radius.y);
				}
			}
			// 右上
			if (objectData[topRightMapchip].isCollision) {
				if (Vector2::CrossProduct(Vector2{ 1, -1 }.normalize(), (topRight - Vector2{ float(int(topRight.x / mapchipSize) * mapchipSize), float(int(topRight.y / mapchipSize) * mapchipSize + mapchipSize) }).normalize()) > 0) {
					transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) + radius.y);
				}
				else {
					transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) - radius.x);
				}
			}
			// 左下
			if (objectData[bottomLeftMapchip].isCollision) {
				if (Vector2::CrossProduct(Vector2{ -1, 1 }.normalize(), (bottomLeft - Vector2{ float(int(bottomLeft.x / mapchipSize) * mapchipSize + mapchipSize), float(int(bottomLeft.y / mapchipSize) * mapchipSize) }).normalize()) > 0) {
					transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) - radius.y);
				}
				else {
					transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) + radius.x);
				}
			}
			// 右下
			if (objectData[bottomRightMapchip].isCollision) {
				if (Vector2::CrossProduct(Vector2{ 1, 1 }.normalize(), (bottomRight - Vector2{ float(int(bottomRight.x / mapchipSize) * mapchipSize), float(int(bottomRight.y / mapchipSize) * mapchipSize) })) > 0) {
					transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) - radius.x);
				}
				else {
					transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) - radius.y);
				}
			}
		}
		// 上下左右のいずれかのみの場合
		else {
			// 上
			if ((objectData[topLeftMapchip].isCollision || objectData[topRightMapchip].isCollision) && velocity->x == 0 && velocity->y != 0) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) + radius.y);
			}

			// 下
			if ((objectData[bottomLeftMapchip].isCollision || objectData[bottomRightMapchip].isCollision) && velocity->x == 0 && velocity->y != 0) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) - radius.y);
			}

			// 左
			if ((objectData[topLeftMapchip].isCollision || objectData[bottomLeftMapchip].isCollision) && velocity->x != 0 && velocity->y == 0) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) + radius.x);
			}

			// 右
			if ((objectData[topRightMapchip].isCollision || objectData[bottomRightMapchip].isCollision) && velocity->x != 0 && velocity->y == 0) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) - radius.x);
			}
		}
	}
	else if (velocity->x != 0 && velocity->y != 0) {
		// 左上
		if (velocity->x < 0 && velocity->y < 0) {
			// 左上のみ壁
			if (objectData[topLeftMapchip].isCollision && !objectData[topRightMapchip].isCollision && !objectData[bottomLeftMapchip].isCollision) {
				if (Vector2::CrossProduct(
					(topLeft - Vector2{ float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize), float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) }).normalize(), { -1, 0 }
				) < (std::sinf)(-PI / 4)) {
					velocity->x = 0;
					transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) + radius.x);
				}
				else {
					velocity->y = 0;
					transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) + radius.y);
				}
			}
			// 上のみ壁
			else if ((objectData[topLeftMapchip].isCollision || objectData[topRightMapchip].isCollision) && !objectData[bottomLeftMapchip].isCollision) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) + radius.y);
			}
			// 左のみ壁
			else if ((objectData[topLeftMapchip].isCollision || objectData[bottomLeftMapchip].isCollision) && !objectData[topRightMapchip].isCollision) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize) * mapchipSize + mapchipSize) + radius.x);
			}
			// それ以外
			else if (!(!objectData[topLeftMapchip].isCollision && !objectData[topRightMapchip].isCollision && !objectData[bottomLeftMapchip].isCollision)) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize) * mapchipSize + mapchipSize) + radius.y);
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) + radius.x);
			}
		}
		// 右上入力
		if (velocity->x > 0 && velocity->y < 0) {
			// 右上のみ壁
			if (objectData[topRightMapchip].isCollision && !objectData[topLeftMapchip].isCollision && !objectData[bottomRightMapchip].isCollision) {
				if (Vector2::CrossProduct(
					(topRight - Vector2{ float(int(topRight.x / mapchipSize) * mapchipSize), float(int(topRight.y / mapchipSize) * mapchipSize + mapchipSize) }).normalize(), { 0, -1 }
				) > (std::sinf)(-PI / 4)) {
					velocity->x = 0;
					transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) - radius.x);
				}
				else {
					velocity->y = 0;
					transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) + radius.y);
				}
			}
			// 上のみ壁
			else if ((objectData[topLeftMapchip].isCollision || objectData[topRightMapchip].isCollision) && !objectData[bottomRightMapchip].isCollision) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) + radius.y);
			}
			// 右のみ壁
			else if ((objectData[topRightMapchip].isCollision || objectData[bottomRightMapchip].isCollision) && !objectData[topLeftMapchip].isCollision) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) - radius.x);
			}
			// それ以外
			else if (!(!objectData[topLeftMapchip].isCollision && !objectData[topRightMapchip].isCollision && !objectData[bottomRightMapchip].isCollision)) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) - radius.x);
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) + radius.y);
			}
		}
		// 右下
		if (velocity->x > 0 && velocity->y > 0) {
			// 右下のみ壁
			if (objectData[bottomRightMapchip].isCollision && !objectData[topRightMapchip].isCollision && !objectData[bottomLeftMapchip].isCollision) {
				if (Vector2::CrossProduct(
					(bottomRight - Vector2{ float(int(bottomRight.x / mapchipSize) * mapchipSize), float(int(bottomRight.y / mapchipSize) * mapchipSize) }).normalize(), { 1, 0 }
				) < (std::sinf)(-PI / 4)) {
					velocity->x = 0;
					transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) - radius.x);
				}
				else {
					velocity->y = 0;
					transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) - radius.y);
				}
			}
			// 下のみ壁
			else if ((objectData[bottomLeftMapchip].isCollision || objectData[bottomRightMapchip].isCollision) && !objectData[topRightMapchip].isCollision) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) - radius.y);
			}
			// 右のみ壁
			else if ((objectData[topRightMapchip].isCollision || objectData[bottomRightMapchip].isCollision) && !objectData[bottomLeftMapchip].isCollision) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) - radius.x);
			}
			// それ以外
			else if (!(!objectData[bottomLeftMapchip].isCollision && !objectData[topRightMapchip].isCollision && !objectData[bottomRightMapchip].isCollision)) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) - radius.x);
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) - radius.y);
			}
		}
		// 左下
		if (velocity->x < 0 && velocity->y > 0) {
			// 左下のみ壁
			if (objectData[bottomLeftMapchip].isCollision && !objectData[topLeftMapchip].isCollision && !objectData[bottomRightMapchip].isCollision) {
				if (Vector2::CrossProduct(
					(bottomLeft - Vector2{ float(int(bottomLeft.x / mapchipSize) * mapchipSize + mapchipSize), float(int(bottomLeft.y / mapchipSize) * mapchipSize) }).normalize(), { 0, 1 }
				) > (std::sinf)(-PI / 4)) {
					velocity->x = 0;
					transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) + radius.x);
				}
				else {
					velocity->y = 0;
					transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) - radius.y);
				}
			}
			// 下のみ壁
			else if ((objectData[bottomLeftMapchip].isCollision || objectData[bottomRightMapchip].isCollision) && !objectData[topLeftMapchip].isCollision) {
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) - radius.y);
			}
			// 左のみ壁
			else if ((objectData[topLeftMapchip].isCollision || objectData[bottomLeftMapchip].isCollision) && !objectData[bottomRightMapchip].isCollision) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) + radius.x);
			}
			// それ以外
			else if (!(!objectData[bottomLeftMapchip].isCollision && !objectData[bottomRightMapchip].isCollision && !objectData[topLeftMapchip].isCollision)) {
				velocity->x = 0;
				transform->set_translatex(float(int(topLeft.x / mapchipSize)* mapchipSize + mapchipSize) + radius.x);
				velocity->y = 0;
				transform->set_translatey(float(int(topLeft.y / mapchipSize)* mapchipSize + mapchipSize) - radius.y);
			}
		}
	}
}

const int Mapchip::GetMapchipSize() const {
	return mapchipSize;
}

const size_t Mapchip::GetRow() const {
	return row;
}

const size_t Mapchip::GetColumn() const {
	return column;
}
