#pragma once

#include <map>
#include <string>
#include <vector>

#include "Transform2D.h"
#include "Vector2D.h"
#include "RenderingObjects2D.h"

struct MapchipData {
	int data;
	std::shared_ptr<Transform2D> transform;
};

struct MapchipObjectData {
	std::string name;
	bool isCollision;
	bool isDraw;
};

class Mapchip {
public:
	Mapchip() = default;

	~Mapchip() = default;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mapchipSize_">1マップチップの大きさ</param>
	/// <param name="spriteFileDir">スプライトのファイルディレク</param>
	/// <param name="objectData_">各マップチップ番号のデータ</param>
	void Initialize(int mapchipSize_, const std::string& spriteFileDir, const std::vector<MapchipObjectData>& objectData_);

	/// <summary>
	/// マップチップのロード
	/// </summary>
	/// <param name="fileDire">ファイルディレクトリ</param>
	void LoadMapchipdata(const std::string& fileDire, bool isReverseRow = false, bool isReverseColumn = false);

	void Begin();

	void BeginRendering();

	void Draw() const;

	void Debug(const Matrix3x3& camera = Transform2D::MakeIdentityMatrix());

	/// <summary>
	/// 4頂点内にblockTypeに指定したブロックが存在するか判定
	/// </summary>
	/// <param name="blockName">判定するブロック名</param>
	/// <param name="pos">位置</param>
	/// <param name="radius">大きさ</param>
	/// <param name="velocity">速度(省略可能)</param>
	/// <returns></returns>
	bool IsBoxInclueBlockName(const std::string& blockName, const Vector2& pos, const Vector2& radius, const Vector2& velocity = { 0, 0 });

	void Collision(std::shared_ptr<Transform2D>& transform, const Vector2& radius, Vector2* velocity);

	const int GetMapchipSize() const;

	const size_t GetRow() const;
	const size_t GetColumn() const;

private:
	std::vector<std::vector<MapchipData>> mapchipData;
	std::string spriteName;
	std::vector<MapchipObjectData> objectData;
	int mapchipSize;

	std::unique_ptr<SequentialSprite2D> sprite;

	Vector2 topLeft;
	Vector2 topRight;
	Vector2 bottomLeft;
	Vector2 bottomRight;

	int topLeftMapchip;
	int topRightMapchip;
	int bottomLeftMapchip;
	int bottomRightMapchip;

	size_t row, column;
};